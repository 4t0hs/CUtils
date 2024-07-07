#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/line/CsvLineCollection.h"
#include "utilities.h"

#define INITIAL_CAPACITY	(32)

static inline bool NeedResize(CsvLineCollection_t *self) {
	return self->length >= self->capacity;
}
void CsvLineCollection_Init(CsvLineCollection_t *self) {
	self->capacity = INITIAL_CAPACITY;
	self->list = calloc(self->capacity, sizeof(CsvLine_t));
	self->length = 0;
}

void CsvLineCollection_Resize(CsvLineCollection_t *self, size_t new_capacity) {
	self->list = realloc(self->list, new_capacity * sizeof(CsvLine_t));
	self->capacity = new_capacity;
}

void CsvLineCollection_MoveAndAdd(CsvLineCollection_t *self, CsvLine_t *move_line) {
	if (NeedResize(self)) {
		CsvLineCollection_Resize(self, self->capacity * 2);
	}
	CsvLine_MoveOwner(move_line, &self->list[self->length++]);
}

void CsvLineCollection_Destroy(CsvLineCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvLine_t *line = &self->list[i];
		CsvLine_Destroy(line);
	}
	free(self->list);
}

void CsvLineCollection_Print(CsvLineCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvLine_t *line = &self->list[i];
		CsvLine_Print(line);
		printf("\n");
	}
}

bool CsvLineCollection_Equals(CsvLineCollection_t *self, CsvLineCollection_t *lines) {
	if (self->length != lines->length) {
		return false;
	}
	for (size_t i = 0; i < self->length; i++) {
		CsvLine_t *line1 = &self->list[i];
		CsvLine_t *line2 = &lines->list[i];
		if (!CsvLine_Equals(line1, line2)) {
			return false;
		}
	}
	return true;
}

