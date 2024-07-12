/**
 * @file Subscription.c
 * @brief サブスクリプション
 * @author atohs
 * @date 2024/07/12
 */
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "utilities.h"
#include "PublisherSubscriber/Subscription.h"

 /**
  * @brief アカウントを取得
  * @param self インスタンス
  * @param id アカウントID
  * @return アカウント
  */
static inline SubscriptionAccount_t *GetAccount(Subscription_t *self, SubscriptionAccountId id) {
	return &self->accounts[id];
}

/**
 * @brief 購読する内容化判定
 * @param self アカウント
 * @param attribute 購読する属性
 * @return
 */
static inline bool IsMatch(SubscriptionAccount_t *self, PublishMessageAttribute attribute) {
	return self->interestedPublish == attribute;
}

/**
 * @brief 初期化
 * @param self インスタンス
 * @param numAccounts アカウント数
 */
void Subscription_Init(Subscription_t *self, size_t numAccounts) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->accounts = calloc(numAccounts, sizeof(SubscriptionAccount_t));
	self->numAccounts = numAccounts;
}

/**
 * @brief サブスクライブ
 * @param self インスタンス
 * @param subscriber サブスクライバー
 * @param interestedTopic 購読する属性
 * @return アカウントID
 */
SubscriptionAccountId Subscription_Contract(Subscription_t *self, const Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	if (UNLIKELY(!self || !subscriber)) {
		return -1;
	}
	for (size_t i = 0; i < self->numAccounts; i++) {
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

/**
 * @brief サブスクライブを停止
 * @param self インスタンス
 * @param id アカウントID
 */
void Subscription_Cancellation(Subscription_t *self, SubscriptionAccountId id) {
	if (UNLIKELY(!self || id >= self->numAccounts)) {
		return;
	}
	SubscriptionAccount_t *account = GetAccount(self, id);
	if (account->contracted) {
		CLEAR(account);
	}
}

/**
 * @brief アカウントを取得
 * @param self インスタンス
 * @param id アカウントID
 * @return アカウント
 */
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

/**
 * @brief 購読する内容にマッチするアカウントを取得
 * @param self インスタンス
 * @param messageAttribute 属性
 * @param matchedIds バッファ
 * @param size バッファサイズ
 * @return マッチしたアカウント数
 */
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

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void Subscription_Destroy(Subscription_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->accounts) free(self->accounts);
	CLEAR(self);
}

/**
 * @brief 契約者数をカウント
 * @param self インスタンス
 * @return 契約者数
 */
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






