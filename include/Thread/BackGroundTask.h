#pragma once

#ifdef __cplusplus
#include <atomic>
#define ATOMIC(type)	std::atomic<type>
extern "C" {
#else
#define ATOMIC(type)	_Atomic type
#endif

#include <pthread.h>
#include <stdbool.h>

struct BackGroundTask_t;
typedef struct BackGroundTask_t BackGroundTask_t;

typedef int BackGroundTaskProgress;

typedef enum BackGroundTaskStatus {
	BACK_GROUND_TASK_FAILURE = -1,
	BACK_GROUND_TASK_NONE,
	BACK_GROUND_TASK_EXITED,
	BACK_GROUND_TASK_CANCELED,
	BACK_GROUND_TASK_ABORTED,
} BackGroundTaskStatus;

typedef struct BackGroundTaskEventHandlers_t {
	void (*setup)(BackGroundTask_t *self);
	void (*progressChanged)(BackGroundTask_t *self, BackGroundTaskProgress newProgress);
	void (*finished)(BackGroundTask_t *self, BackGroundTaskStatus exitStatus);
} BackGroundTaskEventHandlers_t;

typedef struct BackGroundTaskWork_t {
	BackGroundTaskStatus(*func)(BackGroundTask_t *self, void *sender);
	void *sender;
} BackGroundTaskWork_t;

typedef struct BackGroundTaskImpl_t {
	pthread_t id;
	bool isBusy;
	ATOMIC(bool) exited;
	BackGroundTaskWork_t work;
} BackGroundTaskImpl_t;

struct BackGroundTask_t {
	// private
	BackGroundTaskImpl_t impl;
	// public
	char name[16];
	BackGroundTaskEventHandlers_t eventHandlers;
	ATOMIC(BackGroundTaskProgress) progress;
	ATOMIC(BackGroundTaskStatus) exitStatus;
	bool cancellationPending;
};

void BackGroundTask_Init(BackGroundTask_t *self, BackGroundTaskWork_t *work);

int BackGroundTask_Run(BackGroundTask_t *self);

void BackGroundTask_ReportsProgress(BackGroundTask_t *self, BackGroundTaskProgress newProgress);

bool BackGroundTask_IsRunning(BackGroundTask_t *self);

void BackGroundTask_Abort(BackGroundTask_t *self);

void BackGroundTask_Cancel(BackGroundTask_t *self);
void BackGroundTask_Destroy(BackGroundTask_t *self);

#ifdef __cplusplus
}
#endif


