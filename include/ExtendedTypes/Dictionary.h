#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <search.h>
#include <stddef.h>
#include <stdint.h>

typedef struct DictionaryValue_t {
	void *buffer;
	size_t size;
} DictionaryObject_t;

typedef struct DictionaryElement_t {
	char *key;
	DictionaryObject_t object;
} DictionaryElement_t;

typedef struct Dictionary_t {
	struct hsearch_data hashTable;
	struct {
		DictionaryElement_t *list;
		size_t capacity;
		size_t count;
	} elements;
	size_t maxKeySize;
	size_t maxObjectSize;
	void *memoryPool;
} Dictionary_t;

typedef enum DictionaryReturn {
	DICTIONARY_INVALID_ARG = -3,
	DICTIONARY_NOT_FOUND = -2,
	DICTIONARY_FULL = -1,
	DICTIONARY_ADDED = 0,
	DICTIONARY_FOUND = 0,
} DictionaryReturn;

extern void Dictionary_Init(Dictionary_t *self, size_t capacity, size_t maxKeysize, size_t maxObjectSize);
extern DictionaryReturn Dictionary_Add(Dictionary_t *self, const char *key, void *object, size_t objectSize);
extern const DictionaryObject_t *Dictionary_Find(Dictionary_t *self, char *key);
extern void Dictionary_Destroy(Dictionary_t *self);

