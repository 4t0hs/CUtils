/**
 * @file CsvItem.c
 * @brief csvのアイテム
 * @author atohs
 * @date 2024/07/12
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Csv/content/item/CsvItem.h"
#include "utilities.h"

/**
 * @brief 初期化
 * @param self 
 */
void CsvItem_Init(CsvItem_t *self) {
	CLEAR(self);
}

/**
 * @brief 値を設定
 * @param self インスタンス
 * @param value 値
 */
void CsvItem_Set(CsvItem_t *self, char *value) {
	self->text = value;
}

/**
 * @brief ムーブセマンティクス
 * @param self インスタンス
 * @param newOwner 新たなインスタンス
 */
void CsvItem_MoveOwner(CsvItem_t *self, CsvItem_t *newOwner) {
	*newOwner = *self;
	self->text = NULL;
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void CsvItem_Destroy(CsvItem_t *self) {
	CLEAR(self);
}

/**
 * @brief コンソールに出力
 * @param self インスタンス
 */
void CsvItem_Print(CsvItem_t *self) {
	printf("%s", self->text);
}

/**
 * @brief 評価
 * @param self インスタンス
 * @param item アイテム
 * @return 
 */
bool CsvItem_Equals(CsvItem_t *self, CsvItem_t *item) {
	return strcmp(self->text, item->text) ? false : true;
}
