#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include "Subscriber.h"
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


	void Subscription_Init(Subscription_t *self, int numAccounts);

	SubscriptionAccountId Subscription_Contract(Subscription_t *self, Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);

	void Subscription_Cancellation(Subscription_t *self, SubscriptionAccountId id);

	SubscriptionAccount_t *Subscription_GetAccount(Subscription_t *self, SubscriptionAccountId id);

	ssize_t Subscription_Match(Subscription_t *self, PublishMessageAttribute messageAttribute, SubscriptionAccountId matchedIds[], size_t size);

	void Subscription_Destroy(Subscription_t *self);

	size_t Subscription_Count(Subscription_t *self);
#ifdef __cplusplus
}
#endif

