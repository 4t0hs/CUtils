/**
 * @file ArrayList.c
 * @brief リスト
 * @author atohs
 * @date 2024/08/06
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include "ExtendedTypes/ArrayList.h"
#include "ExtendedTypes/MemoryAllocator.h"

#define INITIAL_CAPACITY (64)

 /**
  * @brief
  * @param self
  * @param index
  * @return
  */
static inline ArrayListNode *GetNode(const ArrayList *self, uint64_t index) {
	return &self->list[index];
}

/**
 * @brief
 * @param self
 * @param index
 * @param object
 */
static void Insert(ArrayList *self, uint64_t index, const void *object) {
	ArrayListNode *node = GetNode(self, index);
	node->object = self->allocator.allocate(self->objectSize);
	memcpy(node->object, object, self->objectSize);
	node->hadEntity = true;
	self->length++;
}

/**
 * @brief
 * @param self
 * @param index
 * @param object
 */
static void InsertReference(ArrayList *self, uint64_t index, void *object) {
	ArrayListNode *node = GetNode(self, index);
	node->object = object;
	node->hadEntity = false;
	self->length++;
}

/**
 * @brief
 * @param self
 * @param index
 * @param offset
 * @return
 */
static inline void *Shift(ArrayList *self, uint64_t index, uint64_t offset) {
	return memmove(GetNode(self, index), GetNode(self, index + offset), (self->length - index - offset) * sizeof(ArrayListNode));
}

/**
 * @brief
 * @param self
 * @param node
 */
static inline void ClearNode(ArrayList *self, ArrayListNode *node) {
	if (node->hadEntity) {
		self->allocator.dispose(node->object);
		node->hadEntity = false;
	}
	node->object = NULL;
}

/**
 * @brief
 * @param self
 * @param index
 */
static void RemoveAt(ArrayList *self, uint64_t index) {
	ArrayListNode *node = GetNode(self, index);
	ClearNode(self, node);
	Shift(self, index, 1);
	self->length--;
}

/**
 * @brief
 * @param self
 * @param allocator
 */
void ArrayList_Init(ArrayList *self, const MemoryAllocator_t *allocator, size_t objectSize) {
	if (UNLIKELY(!self || objectSize == 0)) {
		return;
	}
	CLEAR(self);
	self->capacity = INITIAL_CAPACITY;
	self->objectSize = objectSize;
	self->list = calloc(self->capacity, sizeof(ArrayListNode));
	if (allocator) {
		self->allocator = *allocator;
	} else {
		self->allocator = *MemoryAllocator_GetDefault();
	}
}

/**
 * @brief
 * @param self
 */
void ArrayList_Destroy(ArrayList *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	ArrayList_Clear(self);
	free(self->list);
	CLEAR(self);
}

/**
 * @brief
 * @param self
 * @return
 */
bool ArrayList_IsEmpty(ArrayList *self) {
	if (UNLIKELY(!self)) {
		return true;
	}
	return self->length == 0;
}

/**
 * @brief
 * @param self
 * @param newSize
 */
void ArrayList_Resize(ArrayList *self, size_t newSize) {
	if (UNLIKELY(!self || (newSize == 0))) {
		return;
	}
	ArrayListNode *new = realloc(self->list, newSize);
	self->list = new;
	self->capacity = newSize;
}

/**
 * @brief
 * @param self
 * @return
 */
size_t ArrayList_Length(ArrayList *self) {
	if (UNLIKELY(!self)) {
		return 0;
	}
	return self->length;
}

/**
 * @brief
 * @param self
 * @return
 */
size_t ArrayList_Capacity(ArrayList *self) {
	if (UNLIKELY(!self)) {
		return 0;
	}
	return self->capacity;
}

/**
 * @brief
 * @param self
 * @param index
 * @return
 */
const void *ArrayList_At(ArrayList *self, uint64_t index) {
	if (UNLIKELY(!self)) {
		return NULL;
	}
	if (index >= self->length) {
		return NULL;
	}
	return self->list[index].object;
}

/**
 * @brief
 * @param self
 * @param object
 */
void ArrayList_Add(ArrayList *self, const void *object) {
	if (UNLIKELY(!self || !object)) {
		return;
	}
	if (self->length >= self->capacity) {
		ArrayList_Resize(self, self->capacity * 2);
	}
	Insert(self, self->length, object);
}


/**
 * @brief
 * @param self
 * @param object
 */
void ArrayList_AddReference(ArrayList *self, void *object) {
	if (UNLIKELY(!self || !object)) {
		return;
	}
	if (self->length >= self->capacity) {
		ArrayList_Resize(self, self->capacity * 2);
	}
	InsertReference(self, self->length, object);
}

/**
 * @brief
 * @param self
 * @param objects
 * @param length
 */
void ArrayList_AddRange(ArrayList *self, const void *objects, size_t length) {
	if (UNLIKELY(!self || !objects || (length == 0))) {
		return;
	}
	const uint8_t *current = objects;
	for (size_t i = 0; i < length; i++) {
		ArrayList_Add(self, current);
		current += self->objectSize;
	}
}

/**
 * @brief
 * @param self
 * @param list
 */
void ArrayList_Append(ArrayList *self, const ArrayList *list) {
	if (UNLIKELY(!self || !list)) {
		return;
	}
	if (self->objectSize != list->objectSize) {
		return;
	}
	if (self->capacity >= (self->length + list->length)) {
		ArrayList_Resize(self, self->capacity + list->capacity);
	}
	// memcpy(GetNode(self, self->length), list->list, list->length);
	size_t oldLength = self->length;
	for (size_t i = self->length, j = 0; i < (oldLength + list->length); i++, j++) {
		ArrayListNode *otherNode = GetNode(list, j);
		if (otherNode->hadEntity) {
			Insert(self, i, otherNode->object);
		} else {
			InsertReference(self, i, otherNode->object);
		}
	}
}

/**
 * @brief
 * @param self
 * @param index
 * @param object
 */
void ArrayList_Insert(ArrayList *self, uint64_t index, const void *object) {
	if (UNLIKELY(!self || !object)) {
		return;
	}
	if (index >= self->length) {
		return;
	}
	ArrayListNode *node = GetNode(self, index);
	memmove(GetNode(self, index + 1), node, (self->length - index) * sizeof(ArrayListNode));
	Insert(self, index, object);
}

/**
 * @brief
 * @param self
 * @param object
 * @return
 */
ssize_t ArrayList_IndexOf(ArrayList *self, const void *object) {
	if (UNLIKELY(!self || !object)) {
		return -1;
	}
	for (uint64_t i = 0; i < self->length; i++) {
		if (memcmp(GetNode(self, i)->object, object, self->objectSize) == 0) {
			return (ssize_t)i;
		}
	}
	return -1;
}

/**
 * @brief
 * @param self
 * @param object
 * @return
 */
bool ArrayList_Contains(ArrayList *self, const void *object) {
	return ArrayList_IndexOf(self, object) >= 0 ? true : false;
}

/**
 * @brief
 * @param self
 * @param object
 * @return
 */
bool ArrayList_Remove(ArrayList *self, const void *object) {
	if (UNLIKELY(!self || !object)) {
		return false;
	}
	for (uint64_t i = 0; i < self->length; i++) {
		ArrayListNode *node = GetNode(self, i);
		if (memcmp(node->object, object, self->objectSize) == 0) {
			RemoveAt(self, i);
			return true;
		}
	}
	return false;
}

/**
 * @brief
 * @param self
 * @param index
 * @return
 */
bool ArrayList_RemoveAt(ArrayList *self, uint64_t index) {
	if (UNLIKELY(!self)) {
		return false;
	}
	if (index >= self->length) {
		return false;
	}
	RemoveAt(self, index);
	return true;
}

/**
 * @brief
 * @param self
 */
void ArrayList_Clear(ArrayList *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	for (size_t i = 0; i < self->length; i++) {
		ClearNode(self, GetNode(self, i));
	}
	self->length = 0;
}

/**
 * @brief
 * @param self
 * @param callback
 */
void ArrayList_ForEach(ArrayList *self, void (*callback)(uint64_t index, const void *object)) {
	if (UNLIKELY(!self || !callback)) {
		return;
	}
	for (size_t i = 0; i < self->length; i++) {
		callback(i, GetNode(self, i)->object);
	}
}
