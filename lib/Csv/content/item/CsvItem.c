#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Csv/content/item/CsvItem.h"
#include "utilities.h"

void CsvItem_Init(CsvItem_t *self) {
	CLEAR(self);
}

void CsvItem_Set(CsvItem_t *self, char *value) {
	self->text = value;
}

void CsvItem_MoveOwner(CsvItem_t *self, CsvItem_t *new_owner) {
	*new_owner = *self;
	self->text = NULL;
}

void CsvItem_Destroy(CsvItem_t *self) {
	CLEAR(self);
}

void CsvItem_Print(CsvItem_t *self) {
	printf("%s", self->text);
}

bool CsvItem_Equals(CsvItem_t *self, CsvItem_t *item) {
	return strcmp(self->text, item->text) ? false : true;
}
