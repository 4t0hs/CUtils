/**
 * @file Bitset.h
 * @brief ビット集合
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <string.h>

	/**
	 * @brief 制御ブロック
	 */
	typedef struct Bitset_t {
		//! @name Private
		//! @{

		//! ビット集合
		uint64_t *words;
		//! キャパシティー
		uint64_t capacity;
		//! ビット数
		uint64_t numBits;
		//! 文字列バッファ
		char *string;

		//! @}
	} Bitset_t;

	void Bitset_Init(Bitset_t *self, uint64_t numBits);
	void Bitset_Destroy(Bitset_t *self);
	void Bitset_RightShift(Bitset_t *self, uint64_t shift);
	void Bitset_LeftShift(Bitset_t *self, uint64_t shift);
	char *Bitset_ToString(Bitset_t *self);

	#define __BS_WORD_BYTE	((uint8_t)sizeof(*((Bitset_t *)NULL)->words))
	#define __BS_WORD_BIT	((uint64_t)(__BS_WORD_BYTE * 8))
	#define __BS_TO_WORD(v)	(v / __BS_WORD_BIT)
	#define __BS_TO_BIT(v)	(v % __BS_WORD_BIT)
	#define __BS_UNLIKELY(cond)	(__glibc_unlikely(!!(cond)))
	#define __BS_LIKELY(cond)	(__glibc_likely(!!(cond)))

	/**
	 * @brief すべてをゼロクリア
	 * @param self インスタンス
	 */
	inline void Bitset_Clear(Bitset_t *self) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		memset(self->words, 0, self->capacity * sizeof(*self->words));
	}

	/**
	 * @brief 論理積
	 * @param self インスタンス
	 * @param value 
	 */
	inline void Bitset_And(Bitset_t *self, uint64_t value) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		self->words[0] &= value;
	}

	/**
	 * @brief 論理和
	 * @param self 
	 * @param value 
	 */
	inline void Bitset_Or(Bitset_t *self, uint64_t value) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		self->words[0] |= value;
	}

	/**
	 * @brief 排他的論理和
	 * @param self 
	 * @param value 
	 */
	inline void Bitset_ExclusiveOr(Bitset_t *self, uint64_t value) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		self->words[0] ^= value;
	}

	/**
	 * @brief ビットセット
	 * @param self 
	 * @param position ビットの位置
	 */
	inline void Bitset_Set(Bitset_t *self, uint64_t position) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		if (position >= self->numBits) {
			return;
		}
		const uint64_t wordPos = __BS_TO_WORD(position);
		self->words[wordPos] |= (1UL << __BS_TO_BIT(position));
	}

	/**
	 * @brief ビットクリア
	 * @param self 
	 * @param position 
	 */
	inline void Bitset_Reset(Bitset_t *self, uint64_t position) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		if (position >= self->numBits) {
			return;
		}
		const uint64_t wordPos = __BS_TO_WORD(position);
		self->words[wordPos] &= ~(1 << __BS_TO_BIT(position));
	}

	/**
	 * @brief ビット集合の反転を取る
	 * @param self 
	 */
	inline void Bitset_Flip(Bitset_t *self) {
		if (__BS_UNLIKELY(!self)) {
			return;
		}
		for (uint64_t i = 0; i < self->capacity; i++) {
			self->words[i] = ~self->words[i];
		}
	}

	/**
	 * @brief ビット数をカウント
	 * @param self 
	 * @return ビット数
	 */
	inline uint8_t Bitset_Count(Bitset_t *self) {
		if (__BS_UNLIKELY(!self)) {
			return 0;
		}
		uint64_t counter = 0;
		for (uint64_t i = 0; i < self->capacity; i++) {
			counter += __builtin_popcountl(self->words[i]);
		}
		return counter;
	}

	/**
	 * @brief 任意のビットが立っているか
	 * @param self 
	 * @param position 
	 * @return 
	 */
	inline bool Bitset_Test(Bitset_t *self, uint8_t position) {
		if (__BS_UNLIKELY(!self)) {
			return false;
		}
		if (position >= self->numBits) {
			return false;
		}
		const uint64_t wordPos = __BS_TO_WORD(position);
		return self->words[wordPos] & (1 << __BS_TO_BIT(position));
	}

	/**
	 * @brief 一つでもビットが立っているか
	 * @param self 
	 * @return 
	 */
	inline bool Bitset_Any(Bitset_t *self) {
		if (__BS_UNLIKELY(!self)) {
			return false;
		}

		for (uint64_t i = 0; i < self->capacity; i++) {
			if (self->words[i]) {
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief すべてのビットが1か
	 * @param self 
	 * @return 
	 */
	inline bool Bitset_All(Bitset_t *self) {
		if (__BS_UNLIKELY(!self)) {
			return false;
		}

		for (uint64_t i = 0; i < self->capacity; i++) {
			if (self->words[i] != UINT64_MAX) {
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief すべてのビットが0
	 * @param self 
	 * @return 
	 */
	inline bool Bitset_None(Bitset_t *self) {
		if (__BS_UNLIKELY(!self)) {
			return false;
		}
		return !Bitset_Any(self);
	}
#ifdef __cplusplus
}
#endif

