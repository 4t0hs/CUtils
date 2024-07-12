/**
 * @file Publisher.c
 * @brief パブリッシャー
 * @author atohs
 * @date 2024/07/12
 */
#include "utilities.h"
#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/Subscriber.h"

 /**
  * @brief 初期化
  * @param self インスタンス
  * @param maxSubscribers サブスクライバー数
  */
void Publisher_Init(Publisher_t *self, uint8_t maxSubscribers) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	Broker_Init(&self->broker, maxSubscribers);
}

/**
 * @brief サブスクライブ
 * @param self インスタンス
 * @param subscriber 情報
 * @param interestedTopic 購読する属性
 * @return アカウントID
 */
SubscriptionAccountId Publisher_Subscribe(Publisher_t *self, const Subscriber_t *subscriber, PublishMessageAttribute interestedTopic) {
	if (UNLIKELY(!self || !subscriber)) {
		return -1;
	}
	return Broker_Subscribe(&self->broker, subscriber, interestedTopic);
}

/**
 * @brief サブスクライブを停止
 * @param self インスタンス
 * @param id アカウントID
 */
void Publisher_Unsubscribe(Publisher_t *self, SubscriptionAccountId id) {
	if (UNLIKELY(!self)) {
		return;
	}
	Broker_Unsubscribe(&self->broker, id);
}

/**
 * @brief 通知
 * @param self インスタンス
 * @param content 内容
 */
void Publisher_Publish(Publisher_t *self, const PublishContent_t *content) {
	if (UNLIKELY(!self || !content)) {
		return;
	}
	Broker_Publish(&self->broker, content);
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void Publisher_Destroy(Publisher_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	Broker_Destroy(&self->broker);
}
