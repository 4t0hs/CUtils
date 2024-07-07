#include "ExtendedTypes/Dictionary.h"
#include <string.h>
#include <stdlib.h>
#include "utilities.h"

static inline DictionaryReturn Add(Dictionary_t *self, DictionaryElement_t *element) {
	ENTRY e = { .key = element->key, .data = element };
	ENTRY *ep = NULL;
	if (hsearch_r(e, ENTER, &ep, &self->hashTable) == 0) {
		return DICTIONARY_FULL;
	}
	return DICTIONARY_ADDED;
}

static void AssignMemory(Dictionary_t *self) {
	void *current = self->memoryPool;
	self->elements.list = (DictionaryElement_t *)current;

	current += (self->elements.capacity * sizeof(DictionaryElement_t));
	for (size_t i = 0; i < self->elements.capacity; i++) {
		DictionaryElement_t *e = &self->elements.list[i];
		e->key = (char *)current;
		current += self->maxKeySize;
		e->object.buffer = current;
		current += self->maxObjectSize;
	}
}

void Dictionary_Init(Dictionary_t *self, size_t capacity, size_t maxKeySize, size_t maxObjectSize) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->elements.capacity = capacity;
	self->maxKeySize = maxKeySize;
	self->maxObjectSize = maxObjectSize;

	hcreate_r(self->elements.capacity, &self->hashTable);

	size_t allocateSize = (
		self->elements.capacity + self->maxKeySize + self->maxObjectSize
	) * sizeof(DictionaryElement_t);
	self->memoryPool = malloc(allocateSize);
	AssignMemory(self);
}

DictionaryReturn Dictionary_Add(Dictionary_t *self, const char *key, void *object, size_t objectSize) {
	if (UNLIKELY(!self || !key || !object)) {
		return DICTIONARY_INVALID_ARG;
	}
	size_t keySize = strlen(key) + 1;	// NOTE: null文字までコピーするので+1
	if ((keySize > self->maxKeySize) || (objectSize > self->maxObjectSize)) {
		return DICTIONARY_INVALID_ARG;
	}
	if (self->elements.count >= self->elements.capacity) {
		return DICTIONARY_FULL;
	}
	DictionaryElement_t *element = &self->elements.list[self->elements.count];
	memcpy(element->key, key, keySize);
	ENTRY e = { .key = element->key, .data = element };
	ENTRY *ep = NULL;
	if (hsearch_r(e, ENTER, &ep, &self->hashTable) == 0) {
		return DICTIONARY_FULL;
	}
	memcpy(element->object.buffer, object, objectSize);
	element->object.size = objectSize;
	self->elements.count++;
	return DICTIONARY_ADDED;
}

const DictionaryObject_t *Dictionary_Find(Dictionary_t *self, char *key) {
	if (UNLIKELY(!self || !key)) {
		return NULL;
	}
	if ((strlen(key) + 1) > self->maxKeySize) {
		return NULL;
	}
	ENTRY e = { .key = key };
	ENTRY *foundObject = NULL;
	if (hsearch_r(e, FIND, &foundObject, &self->hashTable) == 0) {
		return NULL;
	}
	if (!foundObject) return NULL;
	return &((DictionaryElement_t *)foundObject->data)->object;
}

void Dictionary_Destroy(Dictionary_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	hdestroy_r(&self->hashTable);
	free(self->memoryPool);
}
