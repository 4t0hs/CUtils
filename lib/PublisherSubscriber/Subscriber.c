#include "PublisherSubscriber/Subscriber.h"
#include "PublisherSubscriber/Broker.h"
#include "utilities.h"

void Subscriber_Init(Subscriber_t *this, SubscriberUpdate update, void *userData) {
	CLEAR(this);
	this->update = update;
	this->userData = userData;
}

SubscriberReply Subscriber_Update(Subscriber_t *this, const PublishContent_t *publish) {
	if (this->update) {
		return this->update(publish, this->userData);
	}
	return SUBSCRIBER_ACK;
}

void Subscriber_Destroy(Subscriber_t *this) {
	CLEAR(this);
}



