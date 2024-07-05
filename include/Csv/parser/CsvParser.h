#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include "../property/CsvProperty.h"
#include "../content/CsvContent.h"
#include "../Csv.h"

struct CsvParser;
typedef struct CsvParser CsvParser_t;

extern CsvParser_t *CsvParser_Init(const CsvProperties_t *props);
extern CsvReturnCode CsvParser_LoadFromFile(CsvParser_t *object, const char *filePath);
extern CsvReturnCode CsvParser_LoadFromData(CsvParser_t *object, const char *data, size_t dataSize);
extern const CsvContent_t *CsvParser_GetContent(CsvParser_t *object);
extern void CsvParser_Destroy(CsvParser_t *object);

#ifdef __cplusplus
}
#endif
