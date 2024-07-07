#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/item/CsvItem.h"
#include "Csv/content/item/CsvItemCollection.h"
#include "utilities.h"

#define INITIAL_CAPACITY	(32)

static inline bool NeedResize(CsvItemCollection_t *self) {
	return self->length >= self->capacity;
}

void CsvItemCollection_Init(CsvItemCollection_t *self) {
	CLEAR(self);
	self->capacity = INITIAL_CAPACITY;
	self->list = calloc(self->capacity, sizeof(CsvItem_t));
	self->length = 0;
}

void CsvItemCollection_Resize(CsvItemCollection_t *self, size_t new_capacity) {
	self->list = realloc(self->list, new_capacity * sizeof(CsvItem_t));
	self->capacity = new_capacity;
}

void CsvItemCollection_MoveOwner(CsvItemCollection_t *self, CsvItemCollection_t *new_owner) {
	*new_owner = *self;
	CLEAR(self);
}

void CsvItemCollection_MoveAndAdd(CsvItemCollection_t *self, CsvItem_t *item) {
	if (NeedResize(self)) {
		CsvItemCollection_Resize(self, self->capacity * 2);
	}
	CsvItem_MoveOwner(item, &self->list[self->length++]);
}

void CsvItemCollection_Destroy(CsvItemCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvItem_t *item = &self->list[i];
		CsvItem_Destroy(item);
	}
	free(self->list);
}

void CsvItemCollection_Clear(CsvItemCollection_t *self) {
	CLEAR(self);
}

void CsvItemCollection_Print(CsvItemCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvItem_t *item = &self->list[i];
		CsvItem_Print(item);
		printf(", ");
	}
}

bool CsvItemCollection_Equals(CsvItemCollection_t *self, CsvItemCollection_t *items) {
	if (self->length != items->length) {
		return false;
	}

	for (size_t i = 0; i < self->length; i++) {
		CsvItem_t *item1 = &self->list[i];
		CsvItem_t *item2 = &items->list[i];
		if (!CsvItem_Equals(item1, item2)) {
			return false;
		}
	}
	return true;
}
