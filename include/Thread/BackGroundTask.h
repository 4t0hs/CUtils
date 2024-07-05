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

struct BackGroundTask;
typedef struct BackGroundTask BackGroundTask_t;

typedef int BackGroundTaskProgress;

typedef enum BackGroundTaskStatus {
	BACK_GROUND_TASK_FAILURE = -1,
	BACK_GROUND_TASK_NONE,
	BACK_GROUND_TASK_EXITED,
	BACK_GROUND_TASK_CANCELED,
	BACK_GROUND_TASK_ABORTED,
} BackGroundTaskStatus;

typedef struct BackGroundTaskEventHandlers {
	void (*setup)(BackGroundTask_t *object);
	void (*progressChanged)(BackGroundTask_t *object, BackGroundTaskProgress newProgress);
	void (*finished)(BackGroundTask_t *object, BackGroundTaskStatus exitStatus);
} BackGroundTaskEventHandlers_t;

typedef struct BackGroundTaskWork {
	BackGroundTaskStatus(*func)(BackGroundTask_t *object, void *sender);
	void *sender;
} BackGroundTaskWork_t;

typedef struct BackGroundTaskImpl {
	pthread_t id;
	bool isBusy;
	bool exited;
	BackGroundTaskWork_t work;
} BackGroundTaskImpl_t;

struct BackGroundTask {
	// private
	BackGroundTaskImpl_t impl;

	// public
	char name[16];
	BackGroundTaskEventHandlers_t eventHandlers;
	ATOMIC(BackGroundTaskProgress) progress;
	ATOMIC(BackGroundTaskStatus) exitStatus;
	bool cancellationPending;
};

void BackGroundTask_Init(BackGroundTask_t *object, BackGroundTaskWork_t *work);

int BackGroundTask_Run(BackGroundTask_t *object);

void BackGroundTask_ReportsProgress(BackGroundTask_t *object, BackGroundTaskProgress newProgress);

bool BackGroundTask_IsRunning(BackGroundTask_t *object);

void BackGroundTask_Abort(BackGroundTask_t *object);

void BackGroundTask_Cancel(BackGroundTask_t *object);
void BackGroundTask_Dispose(BackGroundTask_t *object);

#ifdef __cplusplus
}
#endif


