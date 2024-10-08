/**
 * @file CsvItem.h
 * @brief csvアイテム
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

	/**
	 * @brief 制御ブロック
	 */
	typedef struct CsvItem_t {
		//! @name Private
		//! @{
		
		//! データ
		char *text;

		//! @}
	} CsvItem_t;

	extern void CsvItem_Init(CsvItem_t *self);
	extern void CsvItem_Set(CsvItem_t *self, char *value);
	extern void CsvItem_MoveOwner(CsvItem_t *self, CsvItem_t *newOwner);
	extern void CsvItem_Destroy(CsvItem_t *self);
	extern void CsvItem_Print(CsvItem_t *self);
	extern bool CsvItem_Equals(CsvItem_t *self, CsvItem_t *item);
#ifdef __cplusplus
}
#endif


