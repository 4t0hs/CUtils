#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include "utilities.h"
#include "PublisherSubscriber/Subscription.h"

static inline SubscriptionAccount_t *GetAccount(Subscription_t *this, SubscriptionAccountId id) {
	return &this->accounts[id];
}

static inline bool IsMatch(SubscriptionAccount_t *this, PublishMessageAttribute attribute) {
	return this->interestedPublish == attribute;
}

void Subscription_Init(Subscription_t *this, int numAccounts) {
	CLEAR(this);
	this->accounts = calloc(numAccounts, sizeof(SubscriptionAccount_t));
	this->numAccounts = numAccounts;
}

SubscriptionAccountId Subscription_Contract(Subscription_t *this, Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	for (int i = 0; i < this->numAccounts; i++) {
		SubscriptionAccount_t *account = &this->accounts[i];
		if (!account->contracted) {
			account->id = i;
			account->subscriber = *subscriber;
			account->interestedPublish = interestedTopic;
			account->contracted = true;
			return i;
		}
	}
	errno = ENOBUFS;
	return -1;
}

void Subscription_Cancellation(Subscription_t *this, SubscriptionAccountId id) {
	if (id >= this->numAccounts) {
		errno = EINVAL;
		return;
	}
	SubscriptionAccount_t *account = GetAccount(this, id);
	if (account->contracted) {
		CLEAR(account);
	}
}

SubscriptionAccount_t *Subscription_GetAccount(Subscription_t *this, SubscriptionAccountId id) {
	if (id >= this->numAccounts) {
		errno = EINVAL;
		return NULL;
	}
	SubscriptionAccount_t *account = &this->accounts[id];
	if (account->contracted) {
		return account;
	}
	errno = EBADR;
	return NULL;
}

ssize_t Subscription_Match(Subscription_t *this, PublishMessageAttribute topicAttribute, SubscriptionAccountId matchedIds[], size_t size) {
	size_t bufferIndex = 0;
	for (SubscriptionAccountId i = 0; i < this->numAccounts; i++) {
		SubscriptionAccount_t *account = GetAccount(this, i);
		if ((!account->contracted)
			|| (!IsMatch(account, topicAttribute)))
			continue;
		if (size <= bufferIndex) {
			errno = ENOBUFS;
			return -1;
		}
		matchedIds[bufferIndex++] = i;
	}
	return (ssize_t)bufferIndex;
}

void Subscription_Destroy(Subscription_t *this) {
	if (this->accounts) free(this->accounts);
	CLEAR(this);
}

size_t Subscription_Count(Subscription_t *this) {
	size_t counter = 0;
	for (SubscriptionAccountId i = 0; i < this->numAccounts; i++) {
		SubscriptionAccount_t *account = GetAccount(this, i);
		if (account->contracted) {
			counter++;
		}
	}
	return counter;
}






