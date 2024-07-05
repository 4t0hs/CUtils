#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "../content/line/CsvLineCollection.h"
typedef struct CsvContent {
	CsvLineCollection_t lines;
} CsvContent_t;

extern void CsvContent_Init(CsvContent_t *object);
extern void CsvContent_MoveBackLine(CsvContent_t *object, CsvLine_t *line);
extern void CsvContent_Destroy(CsvContent_t *object);
extern void CsvContent_Print(CsvContent_t *object);
extern bool CsvContent_Equals(CsvContent_t *object, CsvContent_t *content);

#ifdef __cplusplus
}
#endif


