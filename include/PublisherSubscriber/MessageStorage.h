#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

	typedef struct MessageStorage_t {
		void *messages;
		size_t messageSize;
		size_t count;
		size_t capacity;
		size_t top, bottom;
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

