/**
 * @file CsvLineCollection.c
 * @brief 行データのコレクション
 * @author atohs
 * @date 2024/07/12
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/line/CsvLineCollection.h"
#include "utilities.h"

 /** @def
  * キャパシティーの初期値
  */
#define INITIAL_CAPACITY	(32)

  /**
   * @brief まだバッファに空きがあるか
   * @param self インスタンス
   * @return
   */
static inline bool NeedResize(CsvLineCollection_t *self) {
	return self->length >= self->capacity;
}

/**
 * @brief 初期化
 * @param self インスタンス
 */
void CsvLineCollection_Init(CsvLineCollection_t *self) {
	self->capacity = INITIAL_CAPACITY;
	self->list = calloc(self->capacity, sizeof(CsvLine_t));
	self->length = 0;
}

/**
 * @brief リサイズ
 * @param self インスタンス
 * @param newCapacity 容量
 */
void CsvLineCollection_Resize(CsvLineCollection_t *self, size_t newCapacity) {
	self->list = realloc(self->list, newCapacity * sizeof(CsvLine_t));
	self->capacity = newCapacity;
}

/**
 * @brief 要素を追加
 * @param self インスタンス
 * @param line 行データ
 */
void CsvLineCollection_MoveAndAdd(CsvLineCollection_t *self, CsvLine_t *line) {
	if (NeedResize(self)) {
		CsvLineCollection_Resize(self, self->capacity * 2);
	}
	CsvLine_MoveOwner(line, &self->list[self->length++]);
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void CsvLineCollection_Destroy(CsvLineCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvLine_t *line = &self->list[i];
		CsvLine_Destroy(line);
	}
	free(self->list);
}

/**
 * @brief コンソールに出力
 * @param self インスタンス
 */
void CsvLineCollection_Print(CsvLineCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvLine_t *line = &self->list[i];
		CsvLine_Print(line);
		printf("\n");
	}
}

/**
 * @brief 評価
 * @param self インスタンス
 * @param lines 対象
 * @return
 */
bool CsvLineCollection_Equals(CsvLineCollection_t *self, CsvLineCollection_t *lines) {
	if (self->length != lines->length) {
		return false;
	}
	for (size_t i = 0; i < self->length; i++) {
		CsvLine_t *line1 = &self->list[i];
		CsvLine_t *line2 = &lines->list[i];
		if (!CsvLine_Equals(line1, line2)) {
			return false;
		}
	}
	return true;
}

