#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "utilities.h"
#include "PublisherSubscriber/Subscription.h"

static inline SubscriptionAccount_t *GetAccount(Subscription_t *self, SubscriptionAccountId id) {
	return &self->accounts[id];
}

static inline bool IsMatch(SubscriptionAccount_t *self, PublishMessageAttribute attribute) {
	return self->interestedPublish == attribute;
}

void Subscription_Init(Subscription_t *self, int numAccounts) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->accounts = calloc(numAccounts, sizeof(SubscriptionAccount_t));
	self->numAccounts = numAccounts;
}

SubscriptionAccountId Subscription_Contract(Subscription_t *self, Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	if (UNLIKELY(!self || !subscriber)) {
		return -1;
	}
	for (int i = 0; i < self->numAccounts; i++) {
		SubscriptionAccount_t *account = &self->accounts[i];
		if (!account->contracted) {
			account->id = i;
			account->subscriber = *subscriber;
			account->interestedPublish = interestedTopic;
			account->contracted = true;
			return i;
		}
	}
	return -1;
}

void Subscription_Cancellation(Subscription_t *self, SubscriptionAccountId id) {
	if (UNLIKELY(!self || id >= self->numAccounts)) {
		return;
	}
	SubscriptionAccount_t *account = GetAccount(self, id);
	if (account->contracted) {
		CLEAR(account);
	}
}

SubscriptionAccount_t *Subscription_GetAccount(Subscription_t *self, SubscriptionAccountId id) {
	if (UNLIKELY(!self || id >= self->numAccounts)) {
		return NULL;
	}
	SubscriptionAccount_t *account = &self->accounts[id];
	if (account->contracted) {
		return account;
	}
	return NULL;
}

ssize_t Subscription_Match(Subscription_t *self, PublishMessageAttribute messageAttribute, SubscriptionAccountId matchedIds[], size_t size) {
	if (UNLIKELY(!self || !matchedIds)) {
		return -1;
	}
	size_t bufferIndex = 0;
	for (SubscriptionAccountId i = 0; i < self->numAccounts; i++) {
		if (size <= bufferIndex) {
			return -1;
		}
		SubscriptionAccount_t *account = GetAccount(self, i);
		if ((!account->contracted) || (!IsMatch(account, messageAttribute))) {
			continue;
		}
		matchedIds[bufferIndex++] = i;
	}
	return (ssize_t)bufferIndex;
}

void Subscription_Destroy(Subscription_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->accounts) free(self->accounts);
	CLEAR(self);
}

size_t Subscription_Count(Subscription_t *self) {
	if (UNLIKELY(!self)) {
		return 0;
	}
	size_t counter = 0;
	for (SubscriptionAccountId i = 0; i < self->numAccounts; i++) {
		SubscriptionAccount_t *account = GetAccount(self, i);
		if (account->contracted) {
			counter++;
		}
	}
	return counter;
}






