#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/CsvContent.h"
#include "Csv/content/line/CsvLineCollection.h"

#define CLEAR(this) (memset(this, 0, sizeof(CsvContent_t)));

void CsvContent_Init(CsvContent_t *this) {
	CLEAR(this);
	CsvLineCollection_Init(&this->lines);
}

void CsvContent_MoveBackLine(CsvContent_t *this, CsvLine_t *line) {
	CsvLineCollection_MoveAndAdd(&this->lines, line);
}

void CsvContent_Destroy(CsvContent_t *this) {
	CsvLineCollection_Destroy(&this->lines);
	CLEAR(this);
}

void CsvContent_Print(CsvContent_t *this) {
	printf("Csv contents\n");
	CsvLineCollection_Print(&this->lines);
}

bool CsvContent_Equals(CsvContent_t *this, CsvContent_t *content) {
	return CsvLineCollection_Equals(&this->lines, &content->lines);
}
