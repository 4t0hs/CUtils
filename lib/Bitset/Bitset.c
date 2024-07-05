#include <stdint.h>
#include <stdlib.h>
#include "utilities.h"
#include "Bitset/Bitset.h"

#define fill(p, v, s)	(memset((p), (v), (s) * __BS_WORD_BYTE))

void Bitset_Init(Bitset_t *object, uint64_t numBits) {
	if (__BS_UNLIKELY(!object)) {
		return;
	}
	CLEAR(object);
	object->numBits = numBits;
	object->capacity = __BS_TO_BIT(numBits) ? __BS_TO_WORD(numBits) + 1 : __BS_TO_WORD(numBits);
	object->words = calloc(object->capacity, sizeof(uint64_t));
}

void Bitset_Destroy(Bitset_t *object) {
	if (__BS_UNLIKELY(!object)) {
		return;
	}
	free(object->words);
	if (object->string)free(object->string);
	CLEAR(object);
}

void Bitset_RightShift(Bitset_t *object, uint64_t shift) {
	if (__BS_UNLIKELY(!object)) {
		return;
	}
	if (shift == 0) {
		return;
	}
	const uint64_t shiftWords = __BS_TO_WORD(shift);
	if (shiftWords >= object->capacity) {
		fill(object->words, 0, object->capacity);
		return;
	}
	const uint64_t offset = __BS_TO_BIT(shift);
	const uint64_t limit = object->capacity - shiftWords - 1;

	if (offset == 0) {
		for (uint64_t i = 0; i <= limit; i++) {
			object->words[i] = object->words[i + shiftWords];
		}
	} else {
		const uint64_t subOffset = __BS_WORD_BIT - offset;
		for (uint64_t i = 0; i < limit; i++) {
			object->words[i] = (
				(object->words[i + shiftWords] >> offset) | (object->words[i + shiftWords + 1] << subOffset)
				);
		}
		object->words[limit] = object->words[limit] >> offset;
	}
	fill(&object->words[limit + 1], 0, object->capacity - limit -1);
}

void Bitset_LeftShift(Bitset_t *object, uint64_t shift) {
	if (__BS_UNLIKELY(!object)) {
		return;
	}
	if (shift == 0) {
		return;
	}
	const uint64_t shiftWords = __BS_TO_WORD(shift);
	if (shiftWords >= object->capacity) {
		fill(object->words, 0, object->capacity);
		return;
	}
	const uint64_t offset = __BS_TO_BIT(shift);

	if (offset == 0) {
		for (uint64_t i = object->capacity - 1; i >= shiftWords; i--) {
			object->words[i] = object->words[i - shiftWords];
		}
	} else {
		const uint64_t subOffset = __BS_WORD_BIT - offset;
		for (uint64_t i = object->capacity - 1; i > shiftWords; i--) {
			object->words[i] = ((object->words[i - shiftWords] << offset) | (object->words[i - shiftWords - 1] >> subOffset));
		}
		object->words[shiftWords] = object->words[0] << offset;
	}
	fill(object->words, 0, shiftWords);
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
char *Bitset_ToString(Bitset_t *object) {
	if (__BS_UNLIKELY(!object)) {
		return NULL;
	}
	if (!object->string) {
		object->string = calloc(object->numBits + 1, 1);
	}
	int64_t numWords = __BS_TO_WORD(object->numBits);
	uint8_t remainBits = __BS_TO_BIT(object->numBits);
	char *str = object->string;
	if (remainBits != 0) {
		BinaryToString(object->words[numWords], str, remainBits);
		str += remainBits;
	}
	for (int64_t i = numWords - 1; i >= 0; i--) {
		BinaryToString(object->words[i], str, __BS_WORD_BIT);
		str += __BS_WORD_BIT;
	}
	*str = '\0';
	return object->string;
}
