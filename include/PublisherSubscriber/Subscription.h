#pragma once

#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include "Subscriber.h"

#ifdef __cplusplus
extern "C" {
#endif
	typedef int SubscriptionAccountId;

	typedef struct SubscriptionAccount_t {
		SubscriptionAccountId id;
		Subscriber_t subscriber;
		PublishMessageAttribute interestedPublish;
		bool contracted;
	} SubscriptionAccount_t;

	typedef struct Subscription_t {
		SubscriptionAccount_t *accounts;
		int numAccounts;
	} Subscription_t;


	void Subscription_Init(Subscription_t *object, int numAccounts);

	SubscriptionAccountId Subscription_Contract(Subscription_t *object, Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);

	void Subscription_Cancellation(Subscription_t *object, SubscriptionAccountId id);

	SubscriptionAccount_t *Subscription_GetAccount(Subscription_t *object, SubscriptionAccountId id);

	ssize_t Subscription_Match(Subscription_t *object, PublishMessageAttribute messageAttribute, SubscriptionAccountId matchedIds[], size_t size);

	void Subscription_Destroy(Subscription_t *object);

	size_t Subscription_Count(Subscription_t *object);
#ifdef __cplusplus
}
#endif

