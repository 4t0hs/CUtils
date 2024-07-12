/**
 * @file ThreadPool.h
 * @brief スレッドプール
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef ATOMIC
#undef ATOMIC
#endif

#ifdef __cplusplus
#include <atomic>
#define ATOMIC(t) std::atomic<t>
extern "C" {
#else
#define ATOMIC(t) _Atomic t
#endif

#include <stdint.h>
#include <glib-2.0/glib.h>

 /**
  * @brief スレッドプールで実行するタスク
  */
typedef struct ThreadPoolTask_t {
	void (*function)(void *arg);
	void *arg;
} ThreadPoolTask_t;

/**
 * @brief スレッドプール
 */
typedef struct ThreadPool_t {
	//! @name Private
	//! @{

	//! タスク
	GQueue *tasks;
	//! ワーカースレッド
	GThread **workers;
	//! 生成するワーカースレッド数
	uint16_t maxNumThreads;
	GMutex mutex;

	//! @}
} ThreadPool_t;

extern void ThreadPool_Init(ThreadPool_t *self, uint16_t numThreads);
extern int ThreadPool_Push(ThreadPool_t *self, const ThreadPoolTask_t *task);
extern int ThreadPool_PushTasks(ThreadPool_t *self, const ThreadPoolTask_t *tasks[], uint64_t numTasks);
extern void ThreadPool_Destroy(ThreadPool_t *self, bool isWait);
extern uint64_t ThreadPool_GetNumTasks(ThreadPool_t *self);

#ifdef __cplusplus
}
#endif

