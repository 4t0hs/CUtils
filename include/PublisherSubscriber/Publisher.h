#pragma once

#include <stddef.h>
#include <stdint.h>
#include "Broker.h"

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct Publisher_t {
		Broker_t broker;
	} Publisher_t;

	void Publisher_Init(Publisher_t *object, uint8_t maxSubscribers);

	SubscriptionAccountId Publisher_Subscribe(Publisher_t *object, Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);

	void Publisher_Unsubscribe(Publisher_t *object, SubscriptionAccountId id);

	void Publisher_Publish(Publisher_t *object, PublishContent_t *topic);

	void Publisher_Destroy(Publisher_t *object);
#ifdef __cplusplus
}
#endif

