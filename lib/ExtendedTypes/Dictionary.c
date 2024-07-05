#include "ExtendedTypes/Dictionary.h"
#include <string.h>
#include <stdlib.h>

#define unlikely(cond)	__glibc_unlikely(!!(cond))
#define likely(cond)	__glibc_likely(!!(cond))

static inline DictionaryReturn Add(Dictionary_t *this, DictionaryElement_t *element) {
	ENTRY e = { .key = element->key, .data = element };
	ENTRY *ep = NULL;
	if (hsearch_r(e, ENTER, &ep, &this->hashTable) == 0) {
		return DICTIONARY_FULL;
	}
	return DICTIONARY_ADDED;
}

void Dictionary_Init(Dictionary_t *this, size_t capacity, size_t maxKeysize, size_t maxObjectSize) {
	memset(this, 0, sizeof(*this));
	hcreate_r(capacity, &this->hashTable);

	size_t allocateSize = (capacity + maxKeysize + maxObjectSize) * sizeof(DictionaryElement_t);
	this->memoryPool = malloc(allocateSize);
	void *current = this->memoryPool;
	this->elements.list = (DictionaryElement_t *)current;
	this->elements.capacity = capacity;

	current += (capacity * sizeof(DictionaryElement_t));
	for (size_t i = 0; i < capacity; i++) {
		DictionaryElement_t *e = &this->elements.list[i];
		e->key = (char *)current;
		current += maxKeysize;
		e->object.buffer = current;
		current += maxObjectSize;
	}
	this->maxKeySize = maxKeysize;
	this->maxObjectSize = maxObjectSize;
}

DictionaryReturn Dictionary_Add(Dictionary_t *this, const char *key, void *object, size_t objectSize) {
	size_t keySize = strlen(key) + 1;	// NOTE: null文字までコピーするので+1
	if (unlikely(!this) || (keySize > this->maxKeySize) || (objectSize > this->maxObjectSize)) {
		return DICTIONARY_INVALID_ARG;
	}
	if (this->elements.count >= this->elements.capacity) {
		return DICTIONARY_FULL;
	}
	DictionaryElement_t *element = &this->elements.list[this->elements.count];
	memcpy(element->key, key, keySize);
	ENTRY e = { .key = element->key, .data = element };
	ENTRY *ep = NULL;
	if (hsearch_r(e, ENTER, &ep, &this->hashTable) == 0) {
		return DICTIONARY_FULL;
	}
	memcpy(element->object.buffer, object, objectSize);
	element->object.size = objectSize;
	this->elements.count++;
	return DICTIONARY_ADDED;
}

const DictionaryObject_t *Dictionary_Find(Dictionary_t *this, char *key) {
	if (unlikely(!this) || ((strlen(key) + 1) > this->maxKeySize)) {
		return NULL;
	}
	ENTRY e = { .key = key };
	ENTRY *foundObject = NULL;
	if (hsearch_r(e, FIND, &foundObject, &this->hashTable) == 0) {
		return NULL;
	}
	if (!foundObject) return NULL;
	return &((DictionaryElement_t *)foundObject->data)->object;
}

void Dictionary_Destroy(Dictionary_t *this) {
	if (unlikely(!this)) return;
	hdestroy_r(&this->hashTable);
	free(this->memoryPool);
}
