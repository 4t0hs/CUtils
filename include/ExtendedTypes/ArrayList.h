/**
 * @file ArrayList.h
 * @brief リスト
 * @author atohs
 * @date 2024/08/06
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "ExtendedTypes/MemoryAllocator.h"

	/**
	 * @brief ノード
	 */
	typedef struct ArrayListNode {
		//! @name private
		//! @{
		void *object;
		bool hadEntity;
		//! @}
	} ArrayListNode;

	/**
	 * @brief 制御ブロック
	 */
	typedef struct ArrayList {
		//! @name private
		//! @{
		ArrayListNode *list;
		size_t objectSize;
		size_t capacity;
		size_t length;
		MemoryAllocator_t allocator;
		//! @}
	} ArrayList;

	//! @name constructor
	//! @{
	extern void ArrayList_Init(ArrayList *self, const MemoryAllocator_t *allocator, size_t objectSize);
	//! @}

	//! @name destructor
	//! @{
	extern void ArrayList_Destroy(ArrayList *self);
	//! @}

	//! @name properties
	//! @{
	extern size_t ArrayList_Capacity(ArrayList *self);
	extern size_t ArrayList_Length(ArrayList *self);
	//! @}

	//! @name methods
	//! @{
	extern bool ArrayList_IsEmpty(ArrayList *self);
	extern void ArrayList_Resize(ArrayList *self, size_t newSize);
	extern const void *ArrayList_At(ArrayList *self, uint64_t index);
	extern void ArrayList_Add(ArrayList *self, const void *object);
	extern void ArrayList_AddReference(ArrayList *self, void *object);
	extern void ArrayList_AddRange(ArrayList *self, const void *objects, size_t length);
	extern void ArrayList_Append(ArrayList *self, const ArrayList *list);
	extern void ArrayList_Insert(ArrayList *self, uint64_t index, const void *object);
	extern ssize_t ArrayList_IndexOf(ArrayList *self, const void *object);
	extern bool ArrayList_Contains(ArrayList *self, const void *object);
	extern bool ArrayList_Remove(ArrayList *self, const void *object);
	extern bool ArrayList_RemoveAt(ArrayList *self, uint64_t index);
	extern void ArrayList_Clear(ArrayList *self);
	extern void ArrayList_ForEach(ArrayList *self, void(*callback)(uint64_t index, const void *object));
	//! @}
#ifdef __cplusplus
}
#endif


