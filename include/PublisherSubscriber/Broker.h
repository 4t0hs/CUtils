#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "Subscription.h"
#include "Subscriber.h"
#include "MessageStorage.h"
#include "PublishContent.h"
	typedef struct Broker_t {
		Subscription_t subscription;
		MessageStorage_t pendingMessages;
		uint8_t maxSubscribers;
	} Broker_t;

	void Broker_Init(Broker_t *self, uint8_t maxSubscribers);

	void Broker_Publish(Broker_t *self, PublishContent_t *content);

	SubscriptionAccountId Broker_Subscribe(Broker_t *self, Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);

	void Broker_Unsubscribe(Broker_t *self, SubscriptionAccountId id);
	void Broker_Destroy(Broker_t *self);

#ifdef __cplusplus
}
#endif

