/**
 * @file CsvContent.c
 * @brief csvデータ
 * @author atohs
 * @date 2024/07/12
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/CsvContent.h"
#include "Csv/content/line/CsvLineCollection.h"
#include "utilities.h"

 /**
  * @brief 初期化
  * @param self インスタンス
  */
void CsvContent_Init(CsvContent_t *self) {
	CLEAR(self);
	CsvLineCollection_Init(&self->lines);
}

/**
 * @brief 初期化
 * @param self インスタンス
 * @param line 行データを追加
 */
void CsvContent_MoveBackLine(CsvContent_t *self, CsvLine_t *line) {
	CsvLineCollection_MoveAndAdd(&self->lines, line);
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void CsvContent_Destroy(CsvContent_t *self) {
	CsvLineCollection_Destroy(&self->lines);
	CLEAR(self);
}

/**
 * @brief コンソールに出力
 * @param self インスタンス
 */
void CsvContent_Print(CsvContent_t *self) {
	printf("Csv contents\n");
	CsvLineCollection_Print(&self->lines);
}

/**
 * @brief 評価
 * @param self インスタンス
 * @param content csvデータ
 * @return
 */
bool CsvContent_Equals(CsvContent_t *self, CsvContent_t *content) {
	return CsvLineCollection_Equals(&self->lines, &content->lines);
}
