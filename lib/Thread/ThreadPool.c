/**
 * @file ThreadPool.c
 * @brief スレッドプール
 * @author atohs
 * @date 2024/07/12
 */
#include <glib-2.0/glib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include "utilities.h"
#include "Thread/ThreadPool.h"

static _Atomic uint64_t poolCounter = 0;

#define MUTEX_INIT(mutex)	(g_mutex_init((mutex)))
#define MUTEX_LOCK(mutex)	(g_mutex_lock((mutex)))
#define MUTEX_UNLOCK(mutex)	(g_mutex_unlock((mutex)))
#define MUTEX_DESTROY(mutex)	(g_mutex_clear((mutex)))

#define COND_INIT(cond)		(g_cond_init((cond)))
#define COND_SIGNAL(cond)	(g_cond_signal((cond)))
#define COND_WAIT(cond, mutex)	(g_cond_wait((cond), (mutex)))
#define COND_BROADCAST(cond)	(g_cond_broadcast((cond)))
#define COND_DESTROY(cond)		(g_cond_clear((cond)))

#define QUEUE_POP	g_queue_pop_head
#define QUEUE_PUSH	g_queue_push_tail

static void _ExitWorker(void *);
//! ワーカースレッドを終了させる
static const ThreadPoolTask_t exitTask = {
	.function = _ExitWorker, .arg = NULL
};

/**
 * @brief 新たなタスクが来るのを待つ
 * @param self インスタンス
 * @return タスク
 */
static ThreadPoolTask_t *WaitForNewTask(ThreadPool_t *self) {
	ThreadPoolTask_t *task;
	while (1) {
		MUTEX_LOCK(&self->mutex);
		task = QUEUE_POP(self->tasks);
		MUTEX_UNLOCK(&self->mutex);
		if (task) {
			break;
		}
		g_usleep(10 * 1000UL);
	}
	return task;
}

/**
 * @brief スレッドの終了
 * @param arg 使用しない
 */
static void _ExitWorker(void *arg) {
	(void)arg;
	g_thread_exit(NULL);
}

/**
 * @brief ワーカースレッドを停止する
 * @param self インスタンス
 */
static inline void StopWorkers(ThreadPool_t *self) {
	for (uint16_t i = 0; i < self->maxNumThreads; i++) {
		MUTEX_LOCK(&self->mutex);
		QUEUE_PUSH(self->tasks, (void *)&exitTask);
		MUTEX_UNLOCK(&self->mutex);
	}
}

/**
 * @brief ワーカースレッド
 * @param arg インスタンス
 * @return 0
 */
static gpointer WorkerThread(gpointer arg) {
	ThreadPool_t *self = arg;
	while (1) {
		ThreadPoolTask_t *task = WaitForNewTask(self);
		if (task) {
			task->function(task->arg);
			free(task);
		}
	}
	return (gpointer)0;
}

/**
 * @brief ワーカーが停止するのを待つ
 * @param self インスタンス
 */
static void WaitForWorkersExited(ThreadPool_t *self) {
	for (uint16_t i = 0; i < self->maxNumThreads; i++) {
		GThread *thread = self->workers[i];
		if (thread) {
			(void)g_thread_join(thread);
		}
	}
}

/**
 * @brief ワーカースレッドを強制停止する
 * @attention 実装が思いついていないので、意味をなしていない
 * @param self インスタンス
 */
static void AbortWorkers(ThreadPool_t *self) {
	StopWorkers(self);
}

/**
 * @brief プロセスの並行性を取得
 * @return プロセッサ数
 */
static uint GetHardwareConcurrency() {
	cpu_set_t cpu_set;
	sched_getaffinity(getpid(), sizeof(cpu_set), &cpu_set);
	return CPU_COUNT(&cpu_set);
}

/**
 * @brief 初期化
 * @param self インスタンス
 * @param numThreads ワーカースレッド数
 */
void ThreadPool_Init(ThreadPool_t *self, uint16_t numThreads) {
	g_return_if_fail(self);

	CLEAR(self);
	if (numThreads > 0) {
		self->maxNumThreads = numThreads;
	} else {
		self->maxNumThreads = GetHardwareConcurrency();
	}

	self->tasks = g_queue_new();
	MUTEX_INIT(&self->mutex);

	self->workers = (GThread **)g_malloc0_n(self->maxNumThreads, sizeof(GThread *));
	for (uint16_t i = 0; i < self->maxNumThreads; i++) {
		char name[16];
		sprintf(name, "ThreadPool_%d", i);
		self->workers[i] = g_thread_new(name, WorkerThread, self);
	}
}

/**
 * @brief タスクをプッシュ
 * @param self インスタンス
 * @param task タスク
 * @return 0: ok
 */
int ThreadPool_Push(ThreadPool_t *self, const ThreadPoolTask_t *task) {
	g_return_val_if_fail(self, -1);
	g_return_val_if_fail(task, -1);
	ThreadPoolTask_t *_task = g_malloc(sizeof(ThreadPoolTask_t));
	*_task = *task;
	MUTEX_LOCK(&self->mutex);
	QUEUE_PUSH(self->tasks, _task);
	MUTEX_UNLOCK(&self->mutex);
	return 0;
}

/**
 * @brief タスクをプッシュ
 * @param self インスタンス
 * @param tasks タスク集合
 * @param numTasks 数
 * @return 0: ok
 */
int ThreadPool_PushTasks(ThreadPool_t *self, const ThreadPoolTask_t *tasks[], uint64_t numTasks) {
	g_return_val_if_fail(self, -1);
	g_return_val_if_fail(tasks, -1);
	g_return_val_if_fail(numTasks, -1);

	for (uint64_t i = 0; i < numTasks; i++) {
		if (ThreadPool_Push(self, tasks[i]) != 0) {
			return -1;
		}
	}
	return 0;
}

/**
 * @brief インスタンスを破棄
 * @attention 'isWait'の値にかかわらずスレッドの終了を待つ
 * @param self インスタンス
 * @param isWait スレッドの終了を待つ
 */
void ThreadPool_Destroy(ThreadPool_t *self, bool isWait) {
	g_return_if_fail(self);
	if (isWait) {
		StopWorkers(self);
	} else {
		AbortWorkers(self);
	}
	WaitForWorkersExited(self);
	g_queue_free(self->tasks);
	if (self->workers) g_free((gpointer)self->workers);
	MUTEX_DESTROY(&self->mutex);
	CLEAR(self);
}

/**
 * @brief タスク数を取得
 * @param self インスタンス
 * @return タスク数
 */
uint64_t ThreadPool_GetNumTasks(ThreadPool_t *self) {
	g_return_val_if_fail(self, 0);
	MUTEX_LOCK(&self->mutex);
	uint64_t numTasks = g_queue_get_length(self->tasks);
	MUTEX_UNLOCK(&self->mutex);
	return numTasks;
}






