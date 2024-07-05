#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/item/CsvItem.h"
#include "Csv/content/item/CsvItemCollection.h"

#define INITIAL_CAPACITY	(32)
#define CLEAR(this)			(memset(this, 0, sizeof(CsvItemCollection_t)));

static inline bool NeedResize(CsvItemCollection_t *this) {
	return this->length >= this->capacity;
}

void CsvItemCollection_Init(CsvItemCollection_t *this) {
	CLEAR(this);
	this->capacity = INITIAL_CAPACITY;
	this->list = calloc(this->capacity, sizeof(CsvItem_t));
	this->length = 0;
}

void CsvItemCollection_Resize(CsvItemCollection_t *this, size_t new_capacity) {
	this->list = realloc(this->list, new_capacity * sizeof(CsvItem_t));
	this->capacity = new_capacity;
}

void CsvItemCollection_MoveOwner(CsvItemCollection_t *this, CsvItemCollection_t *new_owner) {
	*new_owner = *this;
	CLEAR(this);
}

void CsvItemCollection_MoveAndAdd(CsvItemCollection_t *this, CsvItem_t *item) {
	if (NeedResize(this)) {
		CsvItemCollection_Resize(this, this->capacity * 2);
	}
	CsvItem_MoveOwner(item, &this->list[this->length++]);
}

void CsvItemCollection_Destroy(CsvItemCollection_t *this) {
	for (size_t i = 0; i < this->length; i++) {
		CsvItem_t *item = &this->list[i];
		CsvItem_Destroy(item);
	}
	free(this->list);
}

void CsvItemCollection_Clear(CsvItemCollection_t *this) {
	CLEAR(this);
}

void CsvItemCollection_Print(CsvItemCollection_t *this) {
	for (size_t i = 0; i < this->length; i++) {
		CsvItem_t *item = &this->list[i];
		CsvItem_Print(item);
		printf(", ");
	}
}

bool CsvItemCollection_Equals(CsvItemCollection_t *this, CsvItemCollection_t *items) {
	if (this->length != items->length) {
		return false;
	}

	for (size_t i = 0; i < this->length; i++) {
		CsvItem_t *item1 = &this->list[i];
		CsvItem_t *item2 = &items->list[i];
		if (!CsvItem_Equals(item1, item2)) {
			return false;
		}
	}
	return true;
}
