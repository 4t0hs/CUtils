/**
 * @file MemoryAllocator.h
 * @brief メモリアロケーター
 * @author atohs
 * @date 2024/08/05
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
	/**
	 * @brief 制御ブロック
	 */
	typedef struct MemoryAllocator_t {
		//! @name private
		//! @{
		void *(*allocate)(size_t size);
		void (*dispose)(void *ptr);
		//! @}
	} MemoryAllocator_t;

	extern void MemoryAllocator_Init(MemoryAllocator_t *self, void *(*allocate)(size_t), void(*dispose)(void *));

	extern const MemoryAllocator_t *MemoryAllocator_GetDefault();

#ifdef __cplusplus
}
#endif

