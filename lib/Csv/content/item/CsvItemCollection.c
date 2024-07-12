/**
 * @file CsvItemCollection.c
 * @brief アイテムコレクション
 * @author atohs
 * @date 2024/07/12
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Csv/content/item/CsvItem.h"
#include "Csv/content/item/CsvItemCollection.h"
#include "utilities.h"

//! キャパシティの初期値
#define INITIAL_CAPACITY	(32)

 /**
  * @brief リサイズが必要か
  * @param self インスタンス
  * @return
  */
static inline bool NeedResize(CsvItemCollection_t *self) {
	return self->length >= self->capacity;
}

/**
 * @brief 初期化
 * @param self
 */
void CsvItemCollection_Init(CsvItemCollection_t *self) {
	CLEAR(self);
	self->capacity = INITIAL_CAPACITY;
	self->list = calloc(self->capacity, sizeof(CsvItem_t));
	self->length = 0;
}

/**
 * @brief リサイズ
 * @param self インスタンス
 * @param newCapacity サイズ
 */
void CsvItemCollection_Resize(CsvItemCollection_t *self, size_t newCapacity) {
	self->list = realloc(self->list, newCapacity * sizeof(CsvItem_t));
	self->capacity = newCapacity;
}

/**
 * @brief ムーブセマンティクス
 * @param self インスタンス
 * @param newOwner 新たな所有者
 */
void CsvItemCollection_MoveOwner(CsvItemCollection_t *self, CsvItemCollection_t *newOwner) {
	*newOwner = *self;
	CLEAR(self);
}

/**
 * @brief 要素を追加
 * @param self インスタンス
 * @param item アイテム
 */
void CsvItemCollection_MoveAndAdd(CsvItemCollection_t *self, CsvItem_t *item) {
	if (NeedResize(self)) {
		CsvItemCollection_Resize(self, self->capacity * 2);
	}
	CsvItem_MoveOwner(item, &self->list[self->length++]);
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void CsvItemCollection_Destroy(CsvItemCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvItem_t *item = &self->list[i];
		CsvItem_Destroy(item);
	}
	free(self->list);
}

/**
 * @brief 要素をすべて削除
 * @param self インスタンス
 */
void CsvItemCollection_Clear(CsvItemCollection_t *self) {
	memset(self->list, 0, sizeof(*self->list) * self->length);
	self->length = 0;
}

/**
 * @brief コンソールに出力
 * @param self インスタンス
 */
void CsvItemCollection_Print(CsvItemCollection_t *self) {
	for (size_t i = 0; i < self->length; i++) {
		CsvItem_t *item = &self->list[i];
		CsvItem_Print(item);
		printf(", ");
	}
}

/**
 * @brief 評価
 * @param self インスタンス
 * @param items コレクション
 * @return
 */
bool CsvItemCollection_Equals(CsvItemCollection_t *self, CsvItemCollection_t *items) {
	if (self->length != items->length) {
		return false;
	}

	for (size_t i = 0; i < self->length; i++) {
		CsvItem_t *item1 = &self->list[i];
		CsvItem_t *item2 = &items->list[i];
		if (!CsvItem_Equals(item1, item2)) {
			return false;
		}
	}
	return true;
}
