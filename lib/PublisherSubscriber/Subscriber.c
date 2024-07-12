/**
 * @file Subscriber.c
 * @brief サブスクライバー
 * @author atohs
 * @date 2024/07/12
 */
#include "PublisherSubscriber/Subscriber.h"
#include "PublisherSubscriber/Broker.h"
#include "utilities.h"

/**
 * @brief 初期化
 * @param self インスタンス
 * @param update 通知関数
 * @param userData 通知関数に渡すユーザーデータ
 */
void Subscriber_Init(Subscriber_t *self, SubscriberUpdate update, void *userData) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->update = update;
	self->userData = userData;
}

/**
 * @brief 通知
 * @param self インスタンス
 * @param publish 内容
 * @return ACK､NACK
 */
SubscriberReply Subscriber_Update(Subscriber_t *self, const PublishContent_t *publish) {
	if (UNLIKELY(!self)) {
		return SUBSCRIBER_NACK;
	}
	if (self->update) {
		return self->update(publish, self->userData);
	}
	return SUBSCRIBER_ACK;
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void Subscriber_Destroy(Subscriber_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
}



