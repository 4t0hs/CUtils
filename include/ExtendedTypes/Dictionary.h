/**
 * @file Dictionary.h
 * @brief 辞書型
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <search.h>
#include <stddef.h>
#include <stdint.h>

	/**
	 * @brief 辞書のキーに対応するオブジェクト
	 */
	typedef struct DictionaryObject_t {
		//! バッファ
		void *buffer;
		//! サイズ
		size_t size;
	} DictionaryObject_t;

	/**
	 * @brief 要素
	 */
	typedef struct DictionaryElement_t {
		//! キー
		char *key;
		//! オブジェクト
		DictionaryObject_t object;
	} DictionaryElement_t;

	/**
	 * @brief 制御ブロック
	 */
	typedef struct Dictionary_t {
		//! @name Private
		//! @{

		//! ハッシュテーブル
		struct hsearch_data hashTable;
		/**
		 * @brief 要素のリスト
		 */
		struct {
			//! リストの実態
			DictionaryElement_t *list;
			//! キャパシティー
			size_t capacity;
			//! 個数
			size_t count;
		} elements;
		//! キーとなる文字列の最大長
		size_t maxKeySize;
		//! オブジェクトの最大サイズ
		size_t maxObjectSize;
		//! メモリプール
		void *memoryPool;

		//! @}
	} Dictionary_t;

	/**
	 * @enum DictionaryReturn
	 * @brief ステータス
	 */
	typedef enum DictionaryReturn {
		DICTIONARY_INVALID_ARG = -3,
		DICTIONARY_NOT_FOUND = -2,
		DICTIONARY_FULL = -1,
		DICTIONARY_ADDED = 0,
		DICTIONARY_FOUND = 0,
	} DictionaryReturn;

	extern void Dictionary_Init(Dictionary_t *self, size_t capacity, size_t maxKeysize, size_t maxObjectSize);
	extern DictionaryReturn Dictionary_Add(Dictionary_t *self, const char *key, void *object, size_t objectSize);
	extern const DictionaryObject_t *Dictionary_Find(Dictionary_t *self, char *key);
	extern void Dictionary_Destroy(Dictionary_t *self);

#ifdef __cplusplus
}
#endif


