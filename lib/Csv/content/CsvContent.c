#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/CsvContent.h"
#include "Csv/content/line/CsvLineCollection.h"
#include "utilities.h"

void CsvContent_Init(CsvContent_t *self) {
	CLEAR(self);
	CsvLineCollection_Init(&self->lines);
}

void CsvContent_MoveBackLine(CsvContent_t *self, CsvLine_t *line) {
	CsvLineCollection_MoveAndAdd(&self->lines, line);
}

void CsvContent_Destroy(CsvContent_t *self) {
	CsvLineCollection_Destroy(&self->lines);
	CLEAR(self);
}

void CsvContent_Print(CsvContent_t *self) {
	printf("Csv contents\n");
	CsvLineCollection_Print(&self->lines);
}

bool CsvContent_Equals(CsvContent_t *self, CsvContent_t *content) {
	return CsvLineCollection_Equals(&self->lines, &content->lines);
}
