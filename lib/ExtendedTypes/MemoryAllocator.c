/**
 * @file MemoryAllocator.c
 * @brief メモリアロケーター
 * @author atohs
 * @date 2024/08/06
 */
#include <string.h>
#include <stdlib.h>
#include "ExtendedTypes/MemoryAllocator.h"

static const MemoryAllocator_t defaultAllocator = {
	.allocate = malloc, .dispose = free
};

/**
 * @brief
 * @param self
 * @param allocate
 * @param dispose
 */
void MemoryAllocator_Init(MemoryAllocator_t *self, void *(*allocate)(size_t), void (*dispose)(void *)) {
	memset(self, 0, sizeof(*self));
	if (!self->allocate) {
		self->allocate = malloc;
	}
	if (!self->dispose) {
		self->dispose = free;
	}
}

/**
 * @brief
 * @return
 */
const MemoryAllocator_t *MemoryAllocator_GetDefault() {
	return &defaultAllocator;
}
