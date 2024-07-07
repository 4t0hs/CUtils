#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "../content/line/CsvLineCollection.h"
typedef struct CsvContent_t {
	CsvLineCollection_t lines;
} CsvContent_t;

extern void CsvContent_Init(CsvContent_t *self);
extern void CsvContent_MoveBackLine(CsvContent_t *self, CsvLine_t *line);
extern void CsvContent_Destroy(CsvContent_t *self);
extern void CsvContent_Print(CsvContent_t *self);
extern bool CsvContent_Equals(CsvContent_t *self, CsvContent_t *content);

#ifdef __cplusplus
}
#endif


