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

static void Republish(Broker_t *this) {
	MessageStorage_t *pending = &this->pendingMessages;
	Subscription_t *subscription = &this->subscription;
	size_t count = pending->count; // 保留中のメッセージを一回だけ送りたいので、最初のたまっている数を覚えておく
	for (size_t i = 0; i < count; i++) {
		const Delivery_t *delivery = MessageStorage_Peek(pending);
		SubscriptionAccount_t *account = Subscription_GetAccount(subscription, delivery->id);
		if ((account) && (Subscriber_Update(&account->subscriber, &delivery->topic) == SUBSCRIBER_NACK)) {
			MessageStorage_MoveLast(pending);	// peekしたものを最後尾に持っていく
		}
		MessageStorage_RemoveTop(pending);
	}
}


void Broker_Init(Broker_t *this, uint8_t maxSubscribers) {
	if (UNLIKELY(!this)) return;
	CLEAR(this);
	this->maxSubscribers = maxSubscribers;
	Subscription_Init(&this->subscription, maxSubscribers);
	MessageStorage_Init(&this->pendingMessages, sizeof(Delivery_t), maxSubscribers * 16UL);
}

void Broker_Publish(Broker_t *this, PublishContent_t *topic) {
	if (UNLIKELY(!this)) return;

	Republish(this);
	SubscriptionAccountId ids[this->maxSubscribers];
	ssize_t numIds = Subscription_Match(&this->subscription, topic->attribute, ids, this->maxSubscribers);
	if (numIds <= 0) {
		return;
	}
	for (SubscriptionAccountId i = 0; i < numIds; i++) {
		SubscriptionAccount_t *account = Subscription_GetAccount(&this->subscription, i);
		if (Subscriber_Update(&account->subscriber, topic) == SUBSCRIBER_NACK) {
			MessageStorage_Push(&this->pendingMessages, &(Delivery_t){
				.id = account->id, .topic = *topic
			});
		}
	}
}

SubscriptionAccountId Broker_Subscribe(Broker_t *this, Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	return Subscription_Contract(&this->subscription, subscriber, interestedTopic);
}

void Broker_Unsubscribe(Broker_t *this, SubscriptionAccountId id) {
	/**
	 * NOTE: pendingしているメッセージここでは消さず、
	 * Republishするときにアカウントが見つからなければ消すという方法を取る
	 */
	Subscription_Cancellation(&this->subscription, id);
}

void Broker_Destroy(Broker_t *this) {
	Subscription_Destroy(&this->subscription);
	MessageStorage_Destroy(&this->pendingMessages);
}



