
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "CsvItem.h"

typedef struct CsvItemCollection {
	CsvItem_t *list;
	size_t length;
	size_t capacity;
} CsvItemCollection_t;

extern void CsvItemCollection_Init(CsvItemCollection_t *object);
extern void CsvItemCollection_Resize(CsvItemCollection_t *object, size_t newCapacity);
extern void CsvItemCollection_MoveOwner(CsvItemCollection_t *object, CsvItemCollection_t *newOwner);
extern void CsvItemCollection_MoveAndAdd(CsvItemCollection_t *object, CsvItem_t *item);
extern void CsvItemCollection_Destroy(CsvItemCollection_t *object);
extern void CsvItemCollection_Clear(CsvItemCollection_t *object);
extern void CsvItemCollection_Print(CsvItemCollection_t *object);
extern bool CsvItemCollection_Equals(CsvItemCollection_t *object, CsvItemCollection_t *items);

#ifdef __cplusplus
}
#endif

