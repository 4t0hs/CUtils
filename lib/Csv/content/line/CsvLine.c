/**
 * @file CsvLine.c
 * @brief 行のデータ
 * @author atohs
 * @date 2024/07/12
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Csv/content/line/CsvLine.h"
#include "Csv/content/line/CsvLineCollection.h"
#include "utilities.h"

 /**
  * @brief 初期化
  * @param self インスタンス
  */
void CsvLine_Init(CsvLine_t *self) {
	CLEAR(self);
	CsvItemCollection_Init(&self->items);
}

/**
 * @brief ムーブセマンティクス
 * @param self インスタンス
 * @param newOwner 新たな所有者
 */
void CsvLine_MoveOwner(CsvLine_t *self, CsvLine_t *newOwner) {
	*newOwner = *self;
	CLEAR(self);
}

/**
 * @brief 最後尾にアイテムを追加
 * @param self インスタンス
 * @param item アイテム
 */
void CsvLine_MoveBackItem(CsvLine_t *self, CsvItem_t *item) {
	CsvItemCollection_MoveAndAdd(&self->items, item);
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void CsvLine_Destroy(CsvLine_t *self) {
	CsvItemCollection_Destroy(&self->items);
	CLEAR(self);
}

/**
 * @brief コンソールに出力
 * @param self
 */
void CsvLine_Print(CsvLine_t *self) {
	CsvItemCollection_Print(&self->items);
}

/**
 * @brief 評価
 * @param self インスタンス
 * @param line 行データ
 * @return
 */
bool CsvLine_Equals(CsvLine_t *self, CsvLine_t *line) {
	return CsvItemCollection_Equals(&self->items, &line->items);
}

