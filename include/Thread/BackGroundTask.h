/**
 * @file BackGroundTask.h
 * @brief バックグラウンドタスク
 * @author atohs
 * @date 2024/07/12
 */
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
#include <stdint.h>

 /**
  * @brief 制御ブロック
  */
struct BackGroundTask_t;
typedef struct BackGroundTask_t BackGroundTask_t;

/**
 * @brief 進捗状況
 */
typedef uint8_t BackGroundTaskProgress;

/**
 * @brief タスクのステータス
 */
typedef enum BackGroundTaskStatus {
	//! 失敗
	BACK_GROUND_TASK_FAILURE = -1,
	//! 初期
	BACK_GROUND_TASK_NONE,
	//! 正常終了
	BACK_GROUND_TASK_EXITED,
	//! 処理キャンセル
	BACK_GROUND_TASK_CANCELED,
	//! 強制中断
	BACK_GROUND_TASK_ABORTED,
} BackGroundTaskStatus;

/**
 * @brief イベントハンドラ
 */
typedef struct BackGroundTaskEventHandlers_t {
	//! セットアップ
	void (*setup)(BackGroundTask_t *self);
	//! 進捗更新
	void (*progressChanged)(BackGroundTask_t *self, BackGroundTaskProgress newProgress);
	//! タスク終了
	void (*finished)(BackGroundTask_t *self, BackGroundTaskStatus exitStatus);
} BackGroundTaskEventHandlers_t;

/**
 * @brief タスク
 */
typedef struct BackGroundTaskWork_t {
	BackGroundTaskStatus(*func)(BackGroundTask_t *self, void *sender);
	void *sender;
} BackGroundTaskWork_t;

/**
 * @brief 制御ブロック
 */
struct BackGroundTask_t {
	//! @name Private
	//! @{
	
	pthread_t id;
	bool isBusy;
	ATOMIC(bool) exited;
	BackGroundTaskWork_t work;

	//! @}

	//! @name Public
	//! @{

	//! 名前
	char name[16];
	//! イベントハンドラたち
	BackGroundTaskEventHandlers_t eventHandlers;
	//! 進捗
	ATOMIC(BackGroundTaskProgress) progress;
	//! タスクのステータス
	ATOMIC(BackGroundTaskStatus) exitStatus;
	//! キャンセル待ち
	bool cancellationPending;

	//! @}
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


