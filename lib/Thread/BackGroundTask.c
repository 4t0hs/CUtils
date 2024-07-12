/**
 * @file BackGroundTask.c
 * @brief バックグラウンドタスク
 * @author atohs
 * @date 2024/07/12
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "Thread/BackGroundTask.h"
#include "utilities.h"

/**
 * @brief タスク終了
 * @param self インスタンス
 */
static void FinishWork(BackGroundTask_t *self) {
	if (self->eventHandlers.finished) self->eventHandlers.finished(self, self->exitStatus);
}

/**
 * @brief セットアップ
 * @param self インスタンス
 */
static void Setup(BackGroundTask_t *self) {
	if (self->name[0] != '\0')
		pthread_setname_np(self->id, self->name);
	else
		pthread_setname_np(self->id, "BackGroundTask");
	if (self->eventHandlers.setup) self->eventHandlers.setup(self);
}

/**
 * @brief スレッドのクリーンアップハンドラ
 * @param arg インスタンス
 */
static void Exit(void *arg) {
	BackGroundTask_t *self = (BackGroundTask_t *)arg;
	self->exitStatus = BACK_GROUND_TASK_ABORTED;
	self->isBusy = false;
	if (self->eventHandlers.finished) self->eventHandlers.finished(self, self->exitStatus);
	self->exited = true;
}

/**
 * @brief タスクの実行
 * @param self インスタンス
 */
static void DoWork(BackGroundTask_t *self) {
	int oldCancelType;
	self->isBusy = true;
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldCancelType);
	pthread_cleanup_push(Exit, self);
	if (self->work.func) {
		self->exitStatus = self->work.func(self, self->work.sender);
	} else {
		self->exitStatus = BACK_GROUND_TASK_FAILURE;
	}
	pthread_cleanup_pop(0);
	pthread_setcanceltype(oldCancelType, NULL);
	self->isBusy = false;
}

/**
 * @brief タスクを実行するスレッド
 * @param arg インスタンス
 * @return 0
 */
static void *ControlThread(void *arg) {
	BackGroundTask_t *self = (BackGroundTask_t *)arg;
	Setup(self);
	DoWork(self);
	FinishWork(self);
	pthread_detach(self->id);
	self->exited = true;
	return (void *)0;
}

/**
 * @brief 初期化
 * @param self インスタンス
 * @param work タスク
 */
void BackGroundTask_Init(BackGroundTask_t *self, BackGroundTaskWork_t *work) {
	if (UNLIKELY(!self || !work)) {
		return;
	}
	CLEAR(self);
	self->work = *work;
	self->exitStatus = BACK_GROUND_TASK_NONE;
	self->exited = true;
}

/**
 * @brief 実行
 * @param self インスタンス
 * @return 0: 成功
 */
int BackGroundTask_Run(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return -1;
	}
	if (!self->exited) {
		return -1;
	}
	self->exited = false;
	if (pthread_create(&self->id, NULL, ControlThread, self) != 0) {
		return -1;
	}
	return 0;
}

/**
 * @brief 進捗を報告
 * @param self インスタンス
 * @param newProgress 新たな進捗
 */
void BackGroundTask_ReportsProgress(BackGroundTask_t *self, BackGroundTaskProgress newProgress) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->eventHandlers.progressChanged)
		self->eventHandlers.progressChanged(self, newProgress);
	self->progress = newProgress;
}

/**
 * @brief タスクを実行中か
 * @param self インスタンス
 * @return 
 */
bool BackGroundTask_IsRunning(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return false;
	}
	return self->isBusy;
}

/**
 * @brief タスクを中断
 * @param self インスタンス
 */
void BackGroundTask_Abort(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (!self->exited) {
		pthread_cancel(self->id);
	}
}

/**
 * @brief タスクの実行をキャンセル
 * @param self インスタンス
 */
void BackGroundTask_Cancel(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	self->cancellationPending = true;
}

/**
 * @brief 終了
 * @param self インスタンス
 */
void BackGroundTask_Destroy(BackGroundTask_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
}




