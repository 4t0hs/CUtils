#pragma once

#include "Subscription.h"
#include "Subscriber.h"
#include "MessageStorage.h"
#include "PublishContent.h"

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct Broker_t {
		Subscription_t subscription;
		MessageStorage_t pendingMessages;
		uint8_t maxSubscribers;
	} Broker_t;

	void Broker_Init(Broker_t *object, uint8_t maxSubscribers);

	void Broker_Publish(Broker_t *object, PublishContent_t *topic);

	SubscriptionAccountId Broker_Subscribe(Broker_t *object, Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);

	void Broker_Unsubscribe(Broker_t *object, SubscriptionAccountId id);
	void Broker_Destroy(Broker_t *object);

#ifdef __cplusplus
}
#endif

