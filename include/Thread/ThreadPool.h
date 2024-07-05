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

typedef struct ThreadPoolTask_t {
	void (*function)(void *arg);
	void *arg;
} ThreadPoolTask_t;

typedef struct ThreadPool_t {
	GQueue *tasks;
	GThread **workers;
	uint16_t maxNumThreads;
	GMutex mutex;
} ThreadPool_t;

extern void ThreadPool_Init(ThreadPool_t *object, uint16_t numThreads);

extern int ThreadPool_Push(ThreadPool_t *object, const ThreadPoolTask_t *task);

extern int ThreadPool_PushTasks(ThreadPool_t *object, const ThreadPoolTask_t *tasks[], uint64_t numTasks);
extern void ThreadPool_Destroy(ThreadPool_t *object, bool isWait);

extern uint64_t ThreadPool_GetNumTasks(ThreadPool_t *object);

#ifdef __cplusplus
}
#endif

