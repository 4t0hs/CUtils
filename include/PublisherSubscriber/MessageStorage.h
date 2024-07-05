#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct MessageStorage_t {
		void *messages;
		size_t messageSize;
		size_t count;
		size_t capacity;
		size_t top, bottom;
	} MessageStorage_t;

	void MessageStorage_Init(MessageStorage_t *object, size_t messageSize, size_t capacity);

	int MessageStorage_Push(MessageStorage_t *object, const void *adding);

	int MessageStorage_Pop(MessageStorage_t *object, void *buffer);

	const void *MessageStorage_Peek(MessageStorage_t *object);

	void MessageStorage_MoveLast(MessageStorage_t *object);

	void MessageStorage_RemoveTop(MessageStorage_t *object);
	void MessageStorage_Destroy(MessageStorage_t *object);
#ifdef __cplusplus
}
#endif

