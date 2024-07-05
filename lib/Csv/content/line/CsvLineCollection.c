#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/line/CsvLineCollection.h"

#define INITIAL_CAPACITY	(32)
#define CLEAR(this)			(memset(this, 0, sizeof(CsvLineCollection_t)))

static inline bool NeedResize(CsvLineCollection_t *this) {
	return this->length >= this->capacity;
}
void CsvLineCollection_Init(CsvLineCollection_t *this) {
	this->capacity = INITIAL_CAPACITY;
	this->list = calloc(this->capacity, sizeof(CsvLine_t));
	this->length = 0;
}

void CsvLineCollection_Resize(CsvLineCollection_t *this, size_t new_capacity) {
	this->list = realloc(this->list, new_capacity * sizeof(CsvLine_t));
	this->capacity = new_capacity;
}

void CsvLineCollection_MoveAndAdd(CsvLineCollection_t *this, CsvLine_t *move_line) {
	if (NeedResize(this)) {
		CsvLineCollection_Resize(this, this->capacity * 2);
	}
	CsvLine_MoveOwner(move_line, &this->list[this->length++]);
}

void CsvLineCollection_Destroy(CsvLineCollection_t *this) {
	for (size_t i = 0; i < this->length; i++) {
		CsvLine_t *line = &this->list[i];
		CsvLine_Destroy(line);
	}
	free(this->list);
}

void CsvLineCollection_Print(CsvLineCollection_t *this) {
	for (size_t i = 0; i < this->length; i++) {
		CsvLine_t *line = &this->list[i];
		CsvLine_Print(line);
		printf("\n");
	}
}

bool CsvLineCollection_Equals(CsvLineCollection_t *this, CsvLineCollection_t *lines) {
	if (this->length != lines->length) {
		return false;
	}
	for (size_t i = 0; i < this->length; i++) {
		CsvLine_t *line1 = &this->list[i];
		CsvLine_t *line2 = &lines->list[i];
		if (!CsvLine_Equals(line1, line2)) {
			return false;
		}
	}
	return true;
}

