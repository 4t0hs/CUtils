#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "utilities.h"
#include "PublisherSubscriber/Subscriber.h"
#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/Subscription.h"
#include "PublisherSubscriber/MessageStorage.h"
#include "PublisherSubscriber/Broker.h"

typedef struct Delivery_t {
	SubscriptionAccountId id;
	PublishContent_t topic;
} Delivery_t;

static void Republish(Broker_t *self) {
	MessageStorage_t *pending = &self->pendingMessages;
	Subscription_t *subscription = &self->subscription;
	size_t count = pending->count; // 保留中のメッセージを一回だけ送りたいので、最初のたまっている数を覚えておく
	for (size_t i = 0; i < count; i++) {
		const Delivery_t *delivery = MessageStorage_Peek(pending);
		SubscriptionAccount_t *account = Subscription_GetAccount(subscription, delivery->id);
		if (UNLIKELY(!account)) {
			continue;
		}
		if (Subscriber_Update(&account->subscriber, &delivery->topic) == SUBSCRIBER_NACK) {
			MessageStorage_MoveLast(pending);	// peekしたものを最後尾に持っていく
		}
		MessageStorage_RemoveTop(pending);
	}
}


void Broker_Init(Broker_t *self, uint8_t maxSubscribers) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->maxSubscribers = maxSubscribers;
	Subscription_Init(&self->subscription, maxSubscribers);
	MessageStorage_Init(&self->pendingMessages, sizeof(Delivery_t), maxSubscribers * 16UL);
}

void Broker_Publish(Broker_t *self, PublishContent_t *content) {
	if (UNLIKELY(!self || !content)) {
		return;
	}
	Republish(self);
	SubscriptionAccountId ids[self->maxSubscribers];
	ssize_t numIds = Subscription_Match(&self->subscription, content->attribute, ids, self->maxSubscribers);
	if (numIds <= 0) {
		return;
	}
	for (SubscriptionAccountId i = 0; i < numIds; i++) {
		SubscriptionAccount_t *account = Subscription_GetAccount(&self->subscription, i);
		if (UNLIKELY(!account)) {
			continue;
		}
		if (Subscriber_Update(&account->subscriber, content) == SUBSCRIBER_NACK) {
			MessageStorage_Push(&self->pendingMessages, &(Delivery_t){
				.id = account->id, .topic = *content
			});
		}
	}
}

SubscriptionAccountId Broker_Subscribe(Broker_t *self, Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	return Subscription_Contract(&self->subscription, subscriber, interestedTopic);
}

void Broker_Unsubscribe(Broker_t *self, SubscriptionAccountId id) {
	/**
	 * NOTE: pendingしているメッセージここでは消さず、
	 * Republishするときにアカウントが見つからなければ消すという方法を取る
	 */
	Subscription_Cancellation(&self->subscription, id);
}

void Broker_Destroy(Broker_t *self) {
	Subscription_Destroy(&self->subscription);
	MessageStorage_Destroy(&self->pendingMessages);
}



