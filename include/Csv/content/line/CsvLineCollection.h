#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "CsvLine.h"

typedef struct CsvLineCollection {
	CsvLine_t *list;
	size_t length;
	size_t capacity;
} CsvLineCollection_t;

extern void CsvLineCollection_Init(CsvLineCollection_t *object);
extern void CsvLineCollection_Resize(CsvLineCollection_t *object, size_t newCapacity);
extern void CsvLineCollection_MoveAndAdd(CsvLineCollection_t *object, CsvLine_t *moveLine);
extern void CsvLineCollection_Destroy(CsvLineCollection_t *object);
extern void CsvLineCollection_Print(CsvLineCollection_t *object);
extern bool CsvLineCollection_Equals(CsvLineCollection_t *object, CsvLineCollection_t *lines);

#ifdef __cplusplus
}
#endif
