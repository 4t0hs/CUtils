/**
 * @file MessageStorage.h
 * @brief メッセージストレージ
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

	/**
	 * @brief 制御ブロック
	 */
	typedef struct MessageStorage_t {
		//! @name Private
		//! @{

		//! バッファ
		void *messages;
		//! メッセージのサイズ
		size_t messageSize;
		//! たまっている数
		size_t count;
		//! 最大メッセージ数
		size_t capacity;
		//! 位置
		size_t top, bottom;

		//! @}
	} MessageStorage_t;

	void MessageStorage_Init(MessageStorage_t *self, size_t messageSize, size_t capacity);
	int MessageStorage_Push(MessageStorage_t *self, const void *adding);
	int MessageStorage_Pop(MessageStorage_t *self, void *buffer);
	const void *MessageStorage_Peek(MessageStorage_t *self);
	void MessageStorage_MoveLast(MessageStorage_t *self);
	void MessageStorage_RemoveTop(MessageStorage_t *self);
	void MessageStorage_Destroy(MessageStorage_t *self);
#ifdef __cplusplus
}
#endif

