#include "utilities.h"
#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/Subscriber.h"

void Publisher_Init(Publisher_t *self, uint8_t maxSubscribers) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	Broker_Init(&self->broker, maxSubscribers);
}

SubscriptionAccountId Publisher_Subscribe(Publisher_t *self, Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	if (UNLIKELY(!self || !subscriber)) {
		return -1;
	}
	return Broker_Subscribe(&self->broker, subscriber, interestedTopic);
}

void Publisher_Unsubscribe(Publisher_t *self, SubscriptionAccountId id) {
	if (UNLIKELY(!self)) {
		return;
	}
	Broker_Unsubscribe(&self->broker, id);
}

void Publisher_Publish(Publisher_t *self, PublishContent_t *content) {
	if (UNLIKELY(!self || !content)) {
		return;
	}
	Broker_Publish(&self->broker, content);
}

void Publisher_Destroy(Publisher_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	Broker_Destroy(&self->broker);
}
