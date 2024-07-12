
/**
 * @file CsvItemCollection.h
 * @brief アイテムリスト
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "CsvItem.h"

	/**
	 * @brief 制御ブロック
	 */
	typedef struct CsvItemCollection_t {
		//! @name Private
		//! @{

		//! リストの実態
		CsvItem_t *list;
		//! 要素数
		size_t length;
		//! キャパシティー
		size_t capacity;

		//! @}
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

