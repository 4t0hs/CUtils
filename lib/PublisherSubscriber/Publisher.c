#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/Subscriber.h"

void Publisher_Init(Publisher_t *this, uint8_t maxSubscribers) {
	Broker_Init(&this->broker, maxSubscribers);
}

SubscriptionAccountId Publisher_Subscribe(Publisher_t *this, Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	return Broker_Subscribe(&this->broker, subscriber, interestedTopic);
}

void Publisher_Unsubscribe(Publisher_t *this, SubscriptionAccountId id) {
	Broker_Unsubscribe(&this->broker, id);
}

void Publisher_Publish(Publisher_t *this, PublishContent_t *topic) {
	Broker_Publish(&this->broker, topic);
}

void Publisher_Destroy(Publisher_t *this) {
	Broker_Destroy(&this->broker);
}
