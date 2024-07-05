#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "Thread/BackGroundTask.h"

#define likely(cond) __glibc_likely(!!(cond))
#define unlikely(cond) __glibc_unlikely(!!(cond))

static void FinishWork(BackGroundTask_t *this) {
	this->impl.isBusy = false;
	if (this->eventHandlers.finished) this->eventHandlers.finished(this, this->exitStatus);
}

static void Setup(BackGroundTask_t *this) {
	if (this->name[0] != '\0')
		pthread_setname_np(this->impl.id, this->name);
	else
		pthread_setname_np(this->impl.id, "BackGroundTask");
	if (this->eventHandlers.setup) this->eventHandlers.setup(this);
}

static void Exit(void *arg) {
	BackGroundTask_t *this = (BackGroundTask_t *)arg;
	this->exitStatus = BACK_GROUND_TASK_ABORTED;
	this->impl.isBusy = false;
	if (this->eventHandlers.finished) this->eventHandlers.finished(this, this->exitStatus);
	this->impl.exited = true;
}

static void DoWork(BackGroundTask_t *this) {
	int oldCancelType;
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldCancelType);
	pthread_cleanup_push(Exit, this);
	this->impl.isBusy = true;
	this->exitStatus = this->impl.work.func(this, this->impl.work.sender);
	pthread_cleanup_pop(0);
	pthread_setcanceltype(oldCancelType, NULL);
}

static void *ControlThread(void *arg) {
	BackGroundTask_t *this = (BackGroundTask_t *)arg;
	Setup(this);
	DoWork(this);
	FinishWork(this);
	pthread_detach(this->impl.id);
	this->impl.exited = true;
	return (void *)0;
}

void BackGroundTask_Init(BackGroundTask_t *this, BackGroundTaskWork_t *work) {
	if (unlikely(!this)) {
		return;
	}
	if (unlikely(!work) || unlikely(!work->func)) {
		return;
	}
	memset(this, 0, sizeof(*this));
	this->impl.work = *work;
	this->exitStatus = BACK_GROUND_TASK_NONE;
}

int BackGroundTask_Run(BackGroundTask_t *this) {
	errno = 0;
	if (unlikely(!this) || unlikely(!this->impl.work.func)) {
		errno = EINVAL;
		return -1;
	}
	int ret = pthread_create(&this->impl.id, NULL, ControlThread, this);
	if (ret != 0) {
		return -1;
	}
	return 0;
}

void BackGroundTask_ReportsProgress(BackGroundTask_t *this, BackGroundTaskProgress newProgress) {
	if (this->eventHandlers.progressChanged)
		this->eventHandlers.progressChanged(this, newProgress);
	this->progress = newProgress;
}

bool BackGroundTask_IsRunning(BackGroundTask_t *this) {
	return this->impl.isBusy;
}

void BackGroundTask_Abort(BackGroundTask_t *this) {
	if (!this->impl.exited) {
		pthread_cancel(this->impl.id);
	}
}

void BackGroundTask_Cancel(BackGroundTask_t *this) {
	this->cancellationPending = true;
}

void BackGroundTask_Dispose(BackGroundTask_t *this) {
	memset(this, 0, sizeof(*this));
}




