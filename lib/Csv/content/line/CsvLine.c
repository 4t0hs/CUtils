#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Csv/content/line/CsvLine.h"
#include "Csv/content/line/CsvLineCollection.h"

#define CLEAR(this)	(memset(this, 0, sizeof(CsvLine_t)))

void CsvLine_Init(CsvLine_t *this) {
	CLEAR(this);
	CsvItemCollection_Init(&this->items);
}

void CsvLine_MoveOwner(CsvLine_t *this, CsvLine_t *newOwner) {
	*newOwner = *this;
	CLEAR(this);
}

void CsvLine_MoveBackItem(CsvLine_t *this, CsvItem_t *moveItem) {
	CsvItemCollection_MoveAndAdd(&this->items, moveItem);
}

void CsvLine_Destroy(CsvLine_t *this) {
	CsvItemCollection_Destroy(&this->items);
	CLEAR(this);
}

void CsvLine_Print(CsvLine_t *this) {
	CsvItemCollection_Print(&this->items);
}

bool CsvLine_Equals(CsvLine_t *this, CsvLine_t *line) {
	return CsvItemCollection_Equals(&this->items, &line->items);
}

