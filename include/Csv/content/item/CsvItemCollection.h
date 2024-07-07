
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "CsvItem.h"

typedef struct CsvItemCollection_t {
	CsvItem_t *list;
	size_t length;
	size_t capacity;
} CsvItemCollection_t;

extern void CsvItemCollection_Init(CsvItemCollection_t *self);
extern void CsvItemCollection_Resize(CsvItemCollection_t *self, size_t newCapacity);
extern void CsvItemCollection_MoveOwner(CsvItemCollection_t *self, CsvItemCollection_t *newOwner);
extern void CsvItemCollection_MoveAndAdd(CsvItemCollection_t *self, CsvItem_t *item);
extern void CsvItemCollection_Destroy(CsvItemCollection_t *self);
extern void CsvItemCollection_Clear(CsvItemCollection_t *self);
extern void CsvItemCollection_Print(CsvItemCollection_t *self);
extern bool CsvItemCollection_Equals(CsvItemCollection_t *self, CsvItemCollection_t *items);

#ifdef __cplusplus
}
#endif

