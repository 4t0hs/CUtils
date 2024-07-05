#pragma once
#include <chrono>
#include <thread>
#include <cstdio>
#include "gtest/gtest.h"
#include "Thread/ThreadPool.h"

class ThreadPoolTest : public ::testing::Test {
private:
	bool destroyed = false;

protected:
	ThreadPool_t pool;
	virtual void SetUp() {
		ThreadPool_Init(&pool, 0);
	}
	virtual void TearDown() {
		if (!destroyed)
			ThreadPool_Destroy(&pool, false);
	}
	int Push(const ThreadPoolTask_t *task) {
		return ThreadPool_Push(&pool, task);
	}
	int PushTasks(const ThreadPoolTask_t *tasks[], uint64_t numTasks) {
		return ThreadPool_PushTasks(&pool, tasks, numTasks);
	}
	void Destroy(bool wait) {
		ThreadPool_Destroy(&pool, wait);
		destroyed = true;
	}
};

TEST_F(ThreadPoolTest, Push) {
	ThreadPoolTask_t task;
	int testNum = 10;
	static bool called = false;
	static int taskArg = 0;
	task.function = [](void *arg) {
		called = true;
		taskArg = *(int *)arg;
		};
	task.arg = &testNum;
	Push(&task);

	Destroy(true);

	EXPECT_EQ(true, called);
	EXPECT_EQ(testNum, taskArg);
}

TEST_F(ThreadPoolTest, PushTasks) {
	ThreadPoolTask_t task1, task2, task3;
	static bool task1Executed = false;
	static bool task2Executed = false;
	static bool task3Executed = false;
	task1.function = [](void *arg) {
		task1Executed = true;
		};
	task2.function = [](void *arg) {
		task2Executed = true;
		};
	task3.function = [](void *arg) {
		task3Executed = true;
		};
	const ThreadPoolTask_t *tasks[] = {&task1, &task2, &task3};
	ASSERT_EQ(0, PushTasks(tasks, 3));
	Destroy(true);

	ASSERT_EQ(true, task1Executed);
	ASSERT_EQ(true, task2Executed);
	ASSERT_EQ(true, task3Executed);
}

TEST_F(ThreadPoolTest, HeavyProcessing) {
	ThreadPoolTask_t task1, task2, task3;
	static bool task1Executed = false;
	static bool task2Executed = false;
	static bool task3Executed = false;
	task1.function = [](void *arg) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		task1Executed = true;
		};
	task2.function = [](void *arg) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		task2Executed = true;
		};
	task3.function = [](void *arg) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		task3Executed = true;
		};
	
	Push(&task1);
	Push(&task2);
	Push(&task3);
	Destroy(true);
	EXPECT_EQ(true, task1Executed);
	EXPECT_EQ(true, task2Executed);
	EXPECT_EQ(true, task3Executed);
}


