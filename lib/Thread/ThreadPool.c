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

static const ThreadPoolTask_t exitTask = {
	.function = _ExitWorker, .arg = NULL
};

static ThreadPoolTask_t *WaitForNewTask(ThreadPool_t *this) {
	ThreadPoolTask_t *task;
	while (1) {
		MUTEX_LOCK(&this->mutex);
		task = QUEUE_POP(this->tasks);
		MUTEX_UNLOCK(&this->mutex);
		if (task) {
			break;
		}
		g_usleep(10 * 1000UL);
	}
	return task;
}

static void _ExitWorker(void *arg) {
	(void)arg;
	g_thread_exit(NULL);
}

static inline void StopWorkers(ThreadPool_t *this) {
	for (uint16_t i = 0; i < this->maxNumThreads; i++) {
		MUTEX_LOCK(&this->mutex);
		QUEUE_PUSH(this->tasks, (void *)&exitTask);
		MUTEX_UNLOCK(&this->mutex);
	}
}

static gpointer WorkerThread(gpointer arg) {
	ThreadPool_t *this = arg;
	while (1) {
		ThreadPoolTask_t *task = WaitForNewTask(this);
		if (task) {
			task->function(task->arg);
			free(task);
		}
	}
	return (gpointer)0;
}

static void WaitForWorkersExited(ThreadPool_t *this) {
	for (uint16_t i = 0; i < this->maxNumThreads; i++) {
		GThread *thread = this->workers[i];
		if (thread) {
			(void)g_thread_join(thread);
		}
	}
}

static void AbortWorkers(ThreadPool_t *this) {
	StopWorkers(this);
}

static uint GetHardwareConcurrency(void) {
	cpu_set_t cpu_set;
	sched_getaffinity(getpid(), sizeof(cpu_set), &cpu_set);
	return CPU_COUNT(&cpu_set);
}

void ThreadPool_Init(ThreadPool_t *this, uint16_t numThreads) {
	g_return_if_fail(this);

	CLEAR(this);
	if (numThreads > 0) {
		this->maxNumThreads = numThreads;
	} else {
		this->maxNumThreads = GetHardwareConcurrency();
	}

	this->tasks = g_queue_new();
	MUTEX_INIT(&this->mutex);

	this->workers = (GThread **)g_malloc0_n(this->maxNumThreads, sizeof(GThread *));
	for (uint16_t i = 0; i < this->maxNumThreads; i++) {
		char name[16];
		sprintf(name, "ThreadPool_%d", i);
		this->workers[i] = g_thread_new(name, WorkerThread, this);
	}
}

int ThreadPool_Push(ThreadPool_t *this, const ThreadPoolTask_t *task) {
	g_return_val_if_fail(this, -1);
	g_return_val_if_fail(task, -1);
	ThreadPoolTask_t *_task = g_malloc(sizeof(ThreadPoolTask_t));
	*_task = *task;
	MUTEX_LOCK(&this->mutex);
	QUEUE_PUSH(this->tasks, _task);
	MUTEX_UNLOCK(&this->mutex);
	return 0;
}

int ThreadPool_PushTasks(ThreadPool_t *this, const ThreadPoolTask_t *tasks[], uint64_t numTasks) {
	g_return_val_if_fail(this, -1);
	g_return_val_if_fail(tasks, -1);
	g_return_val_if_fail(numTasks, -1);

	for (uint64_t i = 0; i < numTasks; i++) {
		if (ThreadPool_Push(this, tasks[i]) != 0) {
			return -1;
		}
	}
	return 0;
}

void ThreadPool_Destroy(ThreadPool_t *this, bool isWait) {
	g_return_if_fail(this);
	if (isWait) {
		StopWorkers(this);
	} else {
		AbortWorkers(this);
	}
	WaitForWorkersExited(this);
	g_queue_free(this->tasks);
	if (this->workers) g_free((gpointer)this->workers);
	MUTEX_DESTROY(&this->mutex);
	CLEAR(this);
}

uint64_t ThreadPool_GetNumTasks(ThreadPool_t *this) {
	g_return_val_if_fail(this, 0);
	MUTEX_LOCK(&this->mutex);
	uint64_t numTasks = g_queue_get_length(this->tasks);
	MUTEX_UNLOCK(&this->mutex);
	return numTasks;
}






