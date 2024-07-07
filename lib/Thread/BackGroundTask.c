#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "Thread/BackGroundTask.h"
#include "utilities.h"

static void FinishWork(BackGroundTask_t *self) {
	if (self->eventHandlers.finished) self->eventHandlers.finished(self, self->exitStatus);
}

static void Setup(BackGroundTask_t *self) {
	if (self->name[0] != '\0')
		pthread_setname_np(self->impl.id, self->name);
	else
		pthread_setname_np(self->impl.id, "BackGroundTask");
	if (self->eventHandlers.setup) self->eventHandlers.setup(self);
}

static void Exit(void *arg) {
	BackGroundTask_t *self = (BackGroundTask_t *)arg;
	self->exitStatus = BACK_GROUND_TASK_ABORTED;
	self->impl.isBusy = false;
	if (self->eventHandlers.finished) self->eventHandlers.finished(self, self->exitStatus);
	self->impl.exited = true;
}

static void DoWork(BackGroundTask_t *self) {
	int oldCancelType;
	self->impl.isBusy = true;
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldCancelType);
	pthread_cleanup_push(Exit, self);
	if (self->impl.work.func) {
		self->exitStatus = self->impl.work.func(self, self->impl.work.sender);
	} else {
		self->exitStatus = BACK_GROUND_TASK_FAILURE;
	}
	pthread_cleanup_pop(0);
	pthread_setcanceltype(oldCancelType, NULL);
	self->impl.isBusy = false;
}

static void *ControlThread(void *arg) {
	BackGroundTask_t *self = (BackGroundTask_t *)arg;
	Setup(self);
	DoWork(self);
	FinishWork(self);
	pthread_detach(self->impl.id);
	self->impl.exited = true;
	return (void *)0;
}

void BackGroundTask_Init(BackGroundTask_t *self, BackGroundTaskWork_t *work) {
	if (UNLIKELY(!self || !work)) {
		return;
	}
	CLEAR(self);
	self->impl.work = *work;
	self->exitStatus = BACK_GROUND_TASK_NONE;
	self->impl.exited = true;
}

int BackGroundTask_Run(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return -1;
	}
	if (!self->impl.exited) {
		return -1;
	}
	self->impl.exited = false;
	if (pthread_create(&self->impl.id, NULL, ControlThread, self) != 0) {
		return -1;
	}
	return 0;
}

void BackGroundTask_ReportsProgress(BackGroundTask_t *self, BackGroundTaskProgress newProgress) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->eventHandlers.progressChanged)
		self->eventHandlers.progressChanged(self, newProgress);
	self->progress = newProgress;
}

bool BackGroundTask_IsRunning(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return false;
	}
	return self->impl.isBusy;
}

void BackGroundTask_Abort(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (!self->impl.exited) {
		pthread_cancel(self->impl.id);
	}
}

void BackGroundTask_Cancel(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	self->cancellationPending = true;
}

void BackGroundTask_Destroy(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
}




