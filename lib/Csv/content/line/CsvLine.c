#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Csv/content/line/CsvLine.h"
#include "Csv/content/line/CsvLineCollection.h"
#include "utilities.h"

void CsvLine_Init(CsvLine_t *self) {
	CLEAR(self);
	CsvItemCollection_Init(&self->items);
}

void CsvLine_MoveOwner(CsvLine_t *self, CsvLine_t *newOwner) {
	*newOwner = *self;
	CLEAR(self);
}

void CsvLine_MoveBackItem(CsvLine_t *self, CsvItem_t *moveItem) {
	CsvItemCollection_MoveAndAdd(&self->items, moveItem);
}

void CsvLine_Destroy(CsvLine_t *self) {
	CsvItemCollection_Destroy(&self->items);
	CLEAR(self);
}

void CsvLine_Print(CsvLine_t *self) {
	CsvItemCollection_Print(&self->items);
}

bool CsvLine_Equals(CsvLine_t *self, CsvLine_t *line) {
	return CsvItemCollection_Equals(&self->items, &line->items);
}

