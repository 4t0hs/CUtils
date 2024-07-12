/**
 * @file Broker.h
 * @brief ブローカー
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "Subscription.h"
#include "Subscriber.h"
#include "MessageStorage.h"
#include "PublishContent.h"
	/**
	 * @brief 制御ブロック
	 */
	typedef struct Broker_t {
		//! @name Private
		//! @{

		Subscription_t subscription;
		//! 保留したメッセージバッファ
		MessageStorage_t pendingMessages;
		//! サブスクライバーの最大数
		uint8_t maxSubscribers;

		//! @}
	} Broker_t;

	void Broker_Init(Broker_t *self, uint8_t maxSubscribers);
	void Broker_Publish(Broker_t *self, const PublishContent_t *content);
	SubscriptionAccountId Broker_Subscribe(Broker_t *self, const Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);
	void Broker_Unsubscribe(Broker_t *self, SubscriptionAccountId id);
	void Broker_Destroy(Broker_t *self);

#ifdef __cplusplus
}
#endif

