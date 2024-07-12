/**
 * @file Subscriber.h
 * @brief サブスクライバー
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "PublishContent.h"
	/**
	 * @brief 応答
	 */
	typedef enum SubscriberReply {
		SUBSCRIBER_ACK = 0,
		SUBSCRIBER_NACK,
	} SubscriberReply;

	/**
	 * @brief 通知ハンドラ
	 */
	typedef SubscriberReply(*SubscriberUpdate)(const PublishContent_t *publish, void *userData);

	/**
	 * @struct Subscriber_t
	 * @brief 制御ブロック
	 */
	typedef struct Subscriber_t {
		//! @name Private
		//! @{

		//! 通知ハンドラ
		SubscriberUpdate update;
		//! ハンドラに渡すデータ
		void *userData;

		//! @}
	} Subscriber_t;

	void Subscriber_Init(Subscriber_t *self, SubscriberUpdate update, void *userData);
	SubscriberReply Subscriber_Update(Subscriber_t *self, const PublishContent_t *publish);
	void Subscriber_Destroy(Subscriber_t *self);
#ifdef __cplusplus
}
#endif

