/**
 * @file MessageStorage.c
 * @brief メッセージを保管する
 * @author atohs
 * @date 2024/07/12
 */
#include <stdint.h>
#include <stdlib.h>
#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/MessageStorage.h"
#include "utilities.h"

/**
 * @brief 最後尾のデータの参照を取得
 * @param self インスタンス
 * @return ポインタ
 */
static inline void *GetBottom(MessageStorage_t *self) {
	return self->messages + (self->messageSize * self->bottom);
}

/**
 * @brief 先頭のデータの参照を取得
 * @param self インスタンス
 * @return ポインタ
 */
static inline void *GetTop(MessageStorage_t *self) {
	return self->messages + (self->top * self->messageSize);
}

/**
 * @brief 先頭のインデックスを更新
 * @param self インスタンス
 */
static inline void UpdateTop(MessageStorage_t *self) {
	if ((self->top + 1) >= self->capacity) {
		self->top = 0;
	} else {
		self->top++;
	}
}

/**
 * @brief 最後尾のインデックスを更新
 * @param self インスタンス
 */
static inline void UpdateBottom(MessageStorage_t *self) {
	if ((self->bottom + 1) >= self->capacity) {
		self->bottom = 0;
	} else {
		self->bottom++;
	}
}

/**
 * @brief 初期化
 * @param self インスタンス
 * @param messageSize データサイズ
 * @param capacity 最大容量
 */
void MessageStorage_Init(MessageStorage_t *self, size_t messageSize, size_t capacity) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->messages = calloc(capacity, messageSize);
	self->capacity = capacity;
	self->messageSize = messageSize;
}

/**
 * @brief プッシュ
 * @param self インスタンス
 * @param adding 追加する要素
 * @return 0: 成功
 */
int MessageStorage_Push(MessageStorage_t *self, const void *adding) {
	if (UNLIKELY(!self || !adding)) {
		return -1;
	}
	if (self->count == self->capacity) {
		return -1;
	}
	void *message = GetBottom(self);
	memcpy(message, adding, self->messageSize);
	UpdateBottom(self);
	self->count++;
	return 0;
}

/**
 * @brief 取得
 * @param self インスタンス
 * @param buffer バッファ
 * @return 0=成功
 */
int MessageStorage_Pop(MessageStorage_t *self, void *buffer) {
	if (UNLIKELY(!self || !buffer)) {
		return -1;
	}
	if (self->count == 0) {
		return -1;
	}
	memcpy(buffer, GetTop(self), self->messageSize);
	UpdateTop(self);
	self->count--;
	return 0;
}

/**
 * @brief 先頭の参照を取得
 * @param self インスタンス
 * @return ポインタ
 */
const void *MessageStorage_Peek(MessageStorage_t *self) {
	if (UNLIKELY(!self)) {
		return NULL;
	}
	if (self->count == 0) {
		return NULL;
	}
	return GetTop(self);
}

/**
 * @brief 先頭の要素を最後尾へ移動
 * @param self インスタンス
 */
void MessageStorage_MoveLast(MessageStorage_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->count <= 1) {
		return;
	}
	void *topMessage = GetTop(self);
	UpdateTop(self);
	UpdateBottom(self);
	memcpy(GetBottom(self), topMessage, self->messageSize);
}

/**
 * @brief 先頭要素を削除
 * @param self インスタンス
 */
void MessageStorage_RemoveTop(MessageStorage_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->count == 0) {
		return;
	}
	UpdateTop(self);
	self->count--;
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void MessageStorage_Destroy(MessageStorage_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->messages) free(self->messages);
	CLEAR(self);
}
