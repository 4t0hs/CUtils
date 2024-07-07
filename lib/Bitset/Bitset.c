#include <stdint.h>
#include <stdlib.h>
#include "utilities.h"
#include "Bitset/Bitset.h"

#define fill(p, v, s)	(memset((p), (v), (s) * __BS_WORD_BYTE))

void Bitset_Init(Bitset_t *self, uint64_t numBits) {
	if (__BS_UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
	self->numBits = numBits;
	self->capacity = __BS_TO_BIT(numBits) ? __BS_TO_WORD(numBits) + 1 : __BS_TO_WORD(numBits);
	self->words = calloc(self->capacity, sizeof(uint64_t));
}

void Bitset_Destroy(Bitset_t *self) {
	if (__BS_UNLIKELY(!self)) {
		return;
	}
	free(self->words);
	if (self->string)free(self->string);
	CLEAR(self);
}

void Bitset_RightShift(Bitset_t *self, uint64_t shift) {
	if (__BS_UNLIKELY(!self)) {
		return;
	}
	if (shift == 0) {
		return;
	}
	const uint64_t shiftWords = __BS_TO_WORD(shift);
	if (shiftWords >= self->capacity) {
		fill(self->words, 0, self->capacity);
		return;
	}
	const uint64_t offset = __BS_TO_BIT(shift);
	const uint64_t limit = self->capacity - shiftWords - 1;

	if (offset == 0) {
		for (uint64_t i = 0; i <= limit; i++) {
			self->words[i] = self->words[i + shiftWords];
		}
	} else {
		const uint64_t subOffset = __BS_WORD_BIT - offset;
		for (uint64_t i = 0; i < limit; i++) {
			self->words[i] = (
				(self->words[i + shiftWords] >> offset) | (self->words[i + shiftWords + 1] << subOffset)
				);
		}
		self->words[limit] = self->words[limit] >> offset;
	}
	fill(&self->words[limit + 1], 0, self->capacity - limit -1);
}

void Bitset_LeftShift(Bitset_t *self, uint64_t shift) {
	if (__BS_UNLIKELY(!self)) {
		return;
	}
	if (shift == 0) {
		return;
	}
	const uint64_t shiftWords = __BS_TO_WORD(shift);
	if (shiftWords >= self->capacity) {
		fill(self->words, 0, self->capacity);
		return;
	}
	const uint64_t offset = __BS_TO_BIT(shift);

	if (offset == 0) {
		for (uint64_t i = self->capacity - 1; i >= shiftWords; i--) {
			self->words[i] = self->words[i - shiftWords];
		}
	} else {
		const uint64_t subOffset = __BS_WORD_BIT - offset;
		for (uint64_t i = self->capacity - 1; i > shiftWords; i--) {
			self->words[i] = ((self->words[i - shiftWords] << offset) | (self->words[i - shiftWords - 1] >> subOffset));
		}
		self->words[shiftWords] = self->words[0] << offset;
	}
	fill(self->words, 0, shiftWords);
}

static inline void BinaryToString(uint64_t v, char *buffer, uint8_t length) {
	if (length == 0) {
		return;
	}
	uint64_t offset = __BS_WORD_BIT - length;
	for (uint64_t mask = 0x8000000000000000 >> offset; mask; mask >>= 1) {
		*buffer++ = (v & mask) ? '1' : '0';
	}
}
char *Bitset_ToString(Bitset_t *self) {
	if (__BS_UNLIKELY(!self)) {
		return NULL;
	}
	if (!self->string) {
		self->string = calloc(self->numBits + 1, 1);
	}
	int64_t numWords = __BS_TO_WORD(self->numBits);
	uint8_t remainBits = __BS_TO_BIT(self->numBits);
	char *str = self->string;
	if (remainBits != 0) {
		BinaryToString(self->words[numWords], str, remainBits);
		str += remainBits;
	}
	for (int64_t i = numWords - 1; i >= 0; i--) {
		BinaryToString(self->words[i], str, __BS_WORD_BIT);
		str += __BS_WORD_BIT;
	}
	*str = '\0';
	return self->string;
}
