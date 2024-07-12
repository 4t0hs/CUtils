/**
 * @file Publisher.h
 * @brief パブリッシャー
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>
#include "Broker.h"
	/**
	 * @brief 制御ブロック
	 */
	typedef struct Publisher_t {
		//! @name Private
		//! @{

		//! ブローカー
		Broker_t broker;

		//! @}
	} Publisher_t;

	void Publisher_Init(Publisher_t *self, uint8_t maxSubscribers);
	SubscriptionAccountId Publisher_Subscribe(Publisher_t *self, const Subscriber_t *subscriber, PublishMessageAttribute interestedPublish);
	void Publisher_Unsubscribe(Publisher_t *self, SubscriptionAccountId id);
	void Publisher_Publish(Publisher_t *self, const PublishContent_t *publish);
	void Publisher_Destroy(Publisher_t *self);
#ifdef __cplusplus
}
#endif

