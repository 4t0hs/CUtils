#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "../item/CsvItemCollection.h"
typedef struct CsvLine {
	CsvItemCollection_t	items;
} CsvLine_t;

extern void CsvLine_Init(CsvLine_t *object);
extern void CsvLine_MoveOwner(CsvLine_t *object, CsvLine_t *newOwner);
extern void CsvLine_MoveBackItem(CsvLine_t *object, CsvItem_t *item);
extern void CsvLine_Destroy(CsvLine_t *object);
extern void CsvLine_Print(CsvLine_t *object);
extern bool CsvLine_Equals(CsvLine_t *object, CsvLine_t *line);

#ifdef __cplusplus
}
#endif
