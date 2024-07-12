/**
 * @file CsvLine.h
 * @brief csvライン
 * @author atohs
 * @date 2024/07/12
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "../item/CsvItemCollection.h"

	/**
	 * @brief 制御ブロック
	 */
	typedef struct CsvLine_t {
		//! @name Private
		//! @{

		//! アイテムリスト
		CsvItemCollection_t	items;

		//! @}
	} CsvLine_t;

	extern void CsvLine_Init(CsvLine_t *self);
	extern void CsvLine_MoveOwner(CsvLine_t *self, CsvLine_t *newOwner);
	extern void CsvLine_MoveBackItem(CsvLine_t *self, CsvItem_t *item);
	extern void CsvLine_Destroy(CsvLine_t *self);
	extern void CsvLine_Print(CsvLine_t *self);
	extern bool CsvLine_Equals(CsvLine_t *self, CsvLine_t *line);

#ifdef __cplusplus
}
#endif
