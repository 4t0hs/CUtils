#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Csv/content/item/CsvItem.h"

#define CLEAR(this)	(memset(this, 0, sizeof(CsvItem_t)))

void CsvItem_Init(CsvItem_t *this) {
	CLEAR(this);
}

void CsvItem_Set(CsvItem_t *this, char *value) {
	this->text = value;
}

void CsvItem_MoveOwner(CsvItem_t *this, CsvItem_t *new_owner) {
	*new_owner = *this;
	this->text = NULL;
}

void CsvItem_Destroy(CsvItem_t *this) {
	CLEAR(this);
}

void CsvItem_Print(CsvItem_t *this) {
	printf("%s", this->text);
}

bool CsvItem_Equals(CsvItem_t *this, CsvItem_t *item) {
	return strcmp(this->text, item->text) ? false : true;
}
