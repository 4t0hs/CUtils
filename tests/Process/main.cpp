#include <thread>
#include "gtest/gtest.h"
#include "ProcessClass.hpp"

class ProcessTest : public::testing::Test {
protected:
	ProcessClass process;
	std::vector<ProcessClass> processes;
	void SetUp() override {}
	void TearDown() override {}
};

TEST_F(ProcessTest, ExecuteProcess) {
	ProcessInfo info{
		"/home/atohs/C/CUtils/tests/Process/test.sh"
	};

	process.SetStartInfo(info);
	process.Start();
	process.Wait();
	EXPECT_EQ(true, process.HasExited());
	EXPECT_EQ(0, process.ExitCode());

	std::string output = process.StandardOutput();
	std::string error = process.StandardError();
	EXPECT_STREQ("This is standard output", output.c_str());
	EXPECT_STREQ("This is standard error", error.c_str());
}

TEST_F(ProcessTest, ExecuteProcessWithEnvironment) {
	ProcessInfo info{ "printf $FOO", false };
	info.SetEnvironment({ "FOO=BAR" });

	process.SetStartInfo(info);
	process.Start();
	process.Wait();
	EXPECT_EQ(true, process.HasExited());
	EXPECT_EQ(0, process.ExitCode());

	EXPECT_STREQ("BAR", process.StandardOutput().c_str());
}

TEST_F(ProcessTest, TimeoutProcess) {
	ProcessInfo info;
	info.commandLine = "sleep 5";

	process.SetStartInfo(info);
	process.Start();
	bool ret = process.WaitFor(1000);
	EXPECT_EQ(false, ret);
	EXPECT_EQ(false, process.HasExited());
	EXPECT_EQ(-1, process.ExitCode());
}

TEST_F(ProcessTest, eventHandler) {
	void (*onExited)(Process * self) = [](Process *self) {
		std::cout << "onExited: " << self->id << std::endl;
	};
	process.onExited = onExited;
	ProcessInfo info1{ "printf hoge" };

	ProcessClass process2;
	process2.onExited = onExited;
	ProcessInfo info2{ "printf fuga" };

	process.SetStartInfo(info1);
	process.Start();
	process2.SetStartInfo(info2);
	process2.Start();

	std::thread thread1{ [&]() {
		process.Wait();
	} };
	std::thread thread2{ [&]() {
		process2.Wait();
	} };

	thread1.join();
	thread2.join();

	EXPECT_EQ(true, process.HasExited());
	EXPECT_EQ(0, process.ExitCode());
	EXPECT_STREQ("hoge", process.StandardOutput().c_str());

	EXPECT_EQ(true, process2.HasExited());
	EXPECT_EQ(0, process2.ExitCode());
	EXPECT_STREQ("fuga", process2.StandardOutput().c_str());
}

TEST_F(ProcessTest, OutputProperties) {
	ProcessInfo info{ "python3 /home/atohs/C/CUtils/tests/Process/proc.py /home/atohs ProcessUnitTest", true };
	process.SetStartInfo(info);
	process.Start();
	process.Wait();

	process.PrintSummary();

	ProcessInfo info2{ "python3 /home/atohs/C/CUtils/tests/Process/proc.py /home/atohs ProcessUnitTest", true };
	ProcessClass process2;
	process2.SetStartInfo(info2);
	process2.Start();
	process2.WaitFor(10 * 1000);
	
	process2.PrintSummary();
}
