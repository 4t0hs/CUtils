#include <stdint.h>
#include <stdlib.h>
#include "PublisherSubscriber/Publisher.h"
#include "PublisherSubscriber/MessageStorage.h"
#include "utilities.h"

static inline void *GetBottom(MessageStorage_t *self) {
	return self->messages + (self->messageSize * self->bottom);
}

static inline void *GetTop(MessageStorage_t *self) {
	return self->messages + (self->top * self->messageSize);
}

static inline void UpdateTop(MessageStorage_t *self) {
	if ((self->top + 1) >= self->capacity) {
		self->top = 0;
	} else {
		self->top++;
	}
}

static inline void UpdateBottom(MessageStorage_t *self) {
	if ((self->bottom + 1) >= self->capacity) {
		self->bottom = 0;
	} else {
		self->bottom++;
	}
}

void MessageStorage_Init(MessageStorage_t *self, size_t messageSize, size_t capacity) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->messages = calloc(capacity, messageSize);
	self->capacity = capacity;
	self->messageSize = messageSize;
}

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

const void *MessageStorage_Peek(MessageStorage_t *self) {
	if (UNLIKELY(!self)) {
		return NULL;
	}
	if (self->count == 0) {
		return NULL;
	}
	return GetTop(self);
}

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

void MessageStorage_Destroy(MessageStorage_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->messages) free(self->messages);
	CLEAR(self);
}
