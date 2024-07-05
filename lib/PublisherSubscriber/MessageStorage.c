#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/MessageStorage.h"
#include "utilities.h"

static inline void *GetBottom(MessageStorage_t *this) {
	return this->messages + (this->messageSize * this->bottom);
}

static inline void *GetTop(MessageStorage_t *this) {
	return this->messages + (this->top * this->messageSize);
}

static inline void UpdateTop(MessageStorage_t *this) {
	if ((this->top + 1) >= this->capacity) {
		this->top = 0;
	} else {
		this->top++;
	}
}

static inline void UpdateBottom(MessageStorage_t *this) {
	if ((this->bottom + 1) >= this->capacity) {
		this->bottom = 0;
	} else {
		this->bottom++;
	}
}

void MessageStorage_Init(MessageStorage_t *this, size_t messageSize, size_t capacity) {
	CLEAR(this);
	this->messages = calloc(capacity, messageSize);
	this->capacity = capacity;
	this->messageSize = messageSize;
}

int MessageStorage_Push(MessageStorage_t *this, const void *adding) {
	if (this->count == this->capacity) {
		errno = ENOBUFS;
		return -1;
	}
	void *message = GetBottom(this);
	memcpy(message, adding, this->messageSize);
	UpdateBottom(this);
	this->count++;
	return 0;
}

int MessageStorage_Pop(MessageStorage_t *this, void *buffer) {
	if (!buffer) {
		errno = EINVAL;
		return -1;
	}
	if (this->count == 0) {
		errno = EINVAL;
		return -1;
	}
	memcpy(buffer, GetTop(this), this->messageSize);
	UpdateTop(this);
	this->count--;
	return 0;
}

const void *MessageStorage_Peek(MessageStorage_t *this) {
	if (this->count == 0) return NULL;
	return GetTop(this);
}

void MessageStorage_MoveLast(MessageStorage_t *this) {
	if (this->count <= 1) {
		return;
	}
	void *topMessage = GetTop(this);
	UpdateTop(this);
	UpdateBottom(this);
	memcpy(GetBottom(this), topMessage, this->messageSize);
}

void MessageStorage_RemoveTop(MessageStorage_t *this) {
	if (this->count == 0) {
		return;
	}
	UpdateTop(this);
	this->count--;
}

void MessageStorage_Destroy(MessageStorage_t *this) {
	if (UNLIKELY(!this)) {
		return;
	}
	if (this->messages) free(this->messages);
	CLEAR(this);
}
