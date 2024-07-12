/**
 * @file Subscription.h
 * @brief サブスクリプション
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include "Subscriber.h"
	//! サブスクリプションのアカウントID
	typedef size_t SubscriptionAccountId;

	/**
	 * @brief アカウント
	 */
	typedef struct SubscriptionAccount_t {
		//! アカウントID
		SubscriptionAccountId id;
		//! サブスクライバー情報
		Subscriber_t subscriber;
		//! 購読する内容
		PublishMessageAttribute interestedPublish;
		//! 契約済み
		bool contracted;
	} SubscriptionAccount_t;

	/**
	 * @brief 制御ブロック
	 */
	typedef struct Subscription_t {
		//! @name Private
		//! @{

		//! アカウント集合
		SubscriptionAccount_t *accounts;
		//! 登録できる最大アカウント数
		size_t numAccounts;

		//! @}
	} Subscription_t;


	void Subscription_Init(Subscription_t *self, size_t numAccounts);
	SubscriptionAccountId Subscription_Contract(Subscription_t *self, const Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);
	void Subscription_Cancellation(Subscription_t *self, SubscriptionAccountId id);
	SubscriptionAccount_t *Subscription_GetAccount(Subscription_t *self, SubscriptionAccountId id);
	ssize_t Subscription_Match(Subscription_t *self, PublishMessageAttribute messageAttribute, SubscriptionAccountId matchedIds[], size_t size);
	void Subscription_Destroy(Subscription_t *self);
	size_t Subscription_Count(Subscription_t *self);
#ifdef __cplusplus
}
#endif

