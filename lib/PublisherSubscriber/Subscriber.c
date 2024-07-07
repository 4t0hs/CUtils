#include "PublisherSubscriber/Subscriber.h"
#include "PublisherSubscriber/Broker.h"
#include "utilities.h"

void Subscriber_Init(Subscriber_t *self, SubscriberUpdate update, void *userData) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->update = update;
	self->userData = userData;
}

SubscriberReply Subscriber_Update(Subscriber_t *self, const PublishContent_t *publish) {
	if (UNLIKELY(!self)) {
		return SUBSCRIBER_NACK;
	}
	if (self->update) {
		return self->update(publish, self->userData);
	}
	return SUBSCRIBER_ACK;
}

void Subscriber_Destroy(Subscriber_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
}



