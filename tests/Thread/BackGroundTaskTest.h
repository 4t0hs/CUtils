#include <unistd.h>
#include "Thread/BackGroundTask.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <future>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <atomic>

std::mutex mutex;
std::condition_variable cond;

class BackGroundTaskTest :public::testing::Test {
protected:
	static BackGroundTaskStatus worker(BackGroundTask_t *o, void *s) {
		return BACK_GROUND_TASK_EXITED;
	}
	static void completed(BackGroundTask_t *o, BackGroundTaskStatus exitStatus) {
		// printf("completed=%d\n", exitStatus);
		{
			std::lock_guard<std::mutex> lock(mutex);
		}
		cond.notify_one();
	}
	static void progressChanged(BackGroundTask_t *o, BackGroundTaskProgress progress) {
		// printf("Report progress=%d\n", progress);
		{
			std::lock_guard<std::mutex> lock(mutex);
		}
		cond.notify_one();
	}
	BackGroundTask_t task;
	virtual void SetUp() {
		BackGroundTaskWork_t w = { .func = worker, .sender = nullptr };
		BackGroundTask_Init(&task, &w);
	}
	virtual void TearDown() {
		BackGroundTask_Destroy(&task);
	}

	bool TimedWait(int sec) {
		std::cv_status status;
		{
			std::unique_lock<std::mutex> uniq_lk(mutex);
			status = cond.wait_for(uniq_lk, std::chrono::seconds(sec));
		}
		return status == std::cv_status::no_timeout;
	}
	void Wait() {
		{
			std::unique_lock<std::mutex> uniq_lk(mutex);
			cond.wait(uniq_lk);
		}
	}
};

TEST_F(BackGroundTaskTest, NormalFinish) {
	task.impl.work.func = [](BackGroundTask_t *o, void *s) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return BACK_GROUND_TASK_EXITED;
	};
	task.eventHandlers.finished = completed;
	BackGroundTask_Run(&task);
	// ASSERT_EQ(true, TimedWait(2));
	Wait();
	EXPECT_EQ(BACK_GROUND_TASK_EXITED, task.exitStatus);
}

TEST_F(BackGroundTaskTest, ProgressReport) {
	task.impl.work.func = [](BackGroundTask_t *o, void *s) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		int prog = 0;
		for (int i = 0; i < 5; i++) {
			BackGroundTask_ReportsProgress(o, prog += 20);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		return BACK_GROUND_TASK_EXITED;
	};
	task.eventHandlers.finished = completed;
	task.eventHandlers.progressChanged = progressChanged;
	BackGroundTask_Run(&task);
	for (int i = 0; i < 5; i++) {
		Wait();
		EXPECT_EQ((i + 1) * 20, task.progress);
	}
	Wait();
	EXPECT_EQ(BACK_GROUND_TASK_EXITED, task.exitStatus);
}

bool isReady = false;
TEST_F(BackGroundTaskTest, setup) {
	task.impl.work.func = [](BackGroundTask_t *o, void *s) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return BACK_GROUND_TASK_EXITED;
	};
	// bool isReady = false;
	task.eventHandlers.finished = completed;
	task.eventHandlers.setup = [](BackGroundTask_t *o) {
		{
			std::lock_guard<std::mutex> lock(mutex);
			isReady = true;
		}
		cond.notify_one();
	};
	BackGroundTask_Run(&task);
	Wait();
	ASSERT_EQ(true, isReady);
	Wait();
	EXPECT_EQ(BACK_GROUND_TASK_EXITED, task.exitStatus);
}

TEST_F(BackGroundTaskTest, WorkCancellation) {
	task.impl.work.func = [](BackGroundTask_t *o, void *s) {
		while (o->cancellationPending == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		// printf("Canceled!!\n");
		return BACK_GROUND_TASK_CANCELED;
	};
	task.eventHandlers.finished = completed;
	BackGroundTask_Run(&task);
	BackGroundTask_Cancel(&task);
	Wait();
	EXPECT_EQ(BACK_GROUND_TASK_CANCELED, task.exitStatus);
}

TEST_F(BackGroundTaskTest, AbortTask) {
	task.impl.work.func = [](BackGroundTask_t *o, void *s) {
		for (int i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		return BACK_GROUND_TASK_EXITED;
	};
	task.eventHandlers.finished = completed;
	BackGroundTask_Run(&task);
	usleep(1000 * 200);
	BackGroundTask_Abort(&task);
	Wait();
	EXPECT_EQ(BACK_GROUND_TASK_ABORTED, task.exitStatus);
}
