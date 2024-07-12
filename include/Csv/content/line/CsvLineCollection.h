/**
 * @file CsvLineCollection.h
 * @brief 行データのコレクション
 * @author atohs
 * @date 2024/07/12
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "CsvLine.h"

	/**
	 * @brief 制御ブロック
	 */
	typedef struct CsvLineCollection_t {
		//! @name Private
		//! @{

		//! 実体
		CsvLine_t *list;
		//! 要素数
		size_t length;
		//! キャパシティー
		size_t capacity;

		//! @}
	} CsvLineCollection_t;

	extern void CsvLineCollection_Init(CsvLineCollection_t *self);
	extern void CsvLineCollection_Resize(CsvLineCollection_t *self, size_t newCapacity);
	extern void CsvLineCollection_MoveAndAdd(CsvLineCollection_t *self, CsvLine_t *moveLine);
	extern void CsvLineCollection_Destroy(CsvLineCollection_t *self);
	extern void CsvLineCollection_Print(CsvLineCollection_t *self);
	extern bool CsvLineCollection_Equals(CsvLineCollection_t *self, CsvLineCollection_t *lines);

#ifdef __cplusplus
}
#endif
