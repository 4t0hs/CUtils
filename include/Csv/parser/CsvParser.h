/**
 * @file CsvParser.h
 * @brief Csvパーサー
 * @author atohs
 * @date 2024/07/12
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include "../property/CsvProperty.h"
#include "../content/CsvContent.h"
#include "../Csv.h"

	/**
	 * @brief 制御ブロック
	 */
	struct CsvParser_t;
	typedef struct CsvParser_t CsvParser_t;

	extern CsvParser_t *CsvParser_Init(const CsvProperties_t *props);
	extern CsvReturnCode CsvParser_LoadFromFile(CsvParser_t *self, const char *filePath);
	extern CsvReturnCode CsvParser_LoadFromData(CsvParser_t *self, const char *data, size_t dataSize);
	extern const CsvContent_t *CsvParser_GetContent(CsvParser_t *self);
	extern void CsvParser_Destroy(CsvParser_t *self);

#ifdef __cplusplus
}
#endif
