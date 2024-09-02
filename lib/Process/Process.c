/**
 * @file Process.c
 * @brief プロセス
 * @author atohs
 * @date 2024/08/29
 */
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/stat.h>
#include <wait.h>
#include "utilities.h"
#include "ProcessManager.h"
#include "Process/Process.h"

atomic_int processCount = 0;

#define ROOT	"/tmp/process"
#define SHELL_PATH "/bin/sh"
#define SHELL_NAME "sh"
#define OPTION_COMMAND "-c"
#define PROCESS_FAILED (-255)
#define STANDARD_OUTPUT_FILE "standard_output"
#define STANDARD_ERROR_FILE "standard_error"

//! @name static methods
//! @{

/**
 * @brief Create a root directory for the workspace
 */
static inline void CreateRootDirectory() {
	if (!File_Exists(ROOT)) {
		mkdir(ROOT, 0777);
	}
}

/**
 * @brief Create the workspace
 */
static inline void CreateWorkspace(Process *self) {
	sprintf(self->workspace, "%s/%d", ROOT, processCount++);
	mkdir(self->workspace, 0777);
}

/**
 * @brief Remove the workspace
 */
static inline void RemoveWorkspace(Process *self) {
	char path[64] = { 0 };

	sprintf(path, "%s/" STANDARD_OUTPUT_FILE, self->workspace);
	remove(path);

	sprintf(path, "%s/" STANDARD_ERROR_FILE, self->workspace);
	remove(path);

	rmdir(self->workspace);
}
#include <errno.h>
/**
 * @brief child processes exit handler
 */
static void CommonExitedHandler(pid_t pid, int32_t exitStatus, void *data) {
	Process *self = (Process *)data;

	self->exitTime = (double)GetRealTime() / 1000.0;
	if (self->onExited) {
		self->onExited(self);
	}
	pthread_mutex_lock(&self->mutex);
	if (self->terminated) {
		pthread_mutex_unlock(&self->mutex);
		ProcessManager_Unregister(pid);
		return;
	}
	self->terminated = true;
	pthread_mutex_unlock(&self->mutex);
	wait4(self->id, &self->status, 0, &self->usage);
	ProcessManager_Unregister(pid);
}

static void CaptureOutput(const char *standardOutput, const char *standardError) {
	if (standardOutput) {
		int outputFd = open(standardOutput, O_RDWR | O_CREAT | O_TRUNC, 0777);
		dup2(outputFd, STDOUT_FILENO);
	}
	if (standardError) {
		int errorFd = open(standardError, O_RDWR | O_CREAT | O_TRUNC, 0777);
		dup2(errorFd, STDERR_FILENO);
	}
}

/**
 * @brief Execute a process
 */
static void Execute(const ProcessStartInfo *info, const char *outputPath, const char *errorPath) {
	const char *newArgv[] = {
		SHELL_NAME, OPTION_COMMAND, info->commandLine, NULL
	};
	if (!info->disableOutputCapture) {
		CaptureOutput(outputPath, errorPath);
	}
	execve(SHELL_PATH, (char *const *)newArgv, info->environment);
	_exit(PROCESS_FAILED);	// unreachable
}

/**
 * @brief Static processing
 */
static void InitStatic() {
	CreateRootDirectory();
	ProcessManager_Init(CommonExitedHandler);
}

static inline double TimevalToDouble(const struct timeval *time) {
	return (double)time->tv_sec + ((double)((long)(time->tv_usec / 1000)) / 1000.0);
}
//! @}

/**
 * @brief constructor
 */
void Process_Init(Process *self, const ProcessStartInfo *info) {
	InitStatic();

	CLEAR(self);

	self->status = -1;
	self->id = -1;
	if (info) {
		self->info = *info;
	}

	CreateWorkspace(self);

	pthread_mutex_init(&self->mutex, NULL);

	pthread_condattr_t conditionAttribute;
	pthread_condattr_init(&conditionAttribute);
	pthread_condattr_setclock(&conditionAttribute, CLOCK_REALTIME);
	pthread_cond_init(&self->condition, &conditionAttribute);
	pthread_condattr_destroy(&conditionAttribute);
}

void Process_Execute(Process *self, const ProcessStartInfo *info) {
	if (UNLIKELY(!self || !info || !info->commandLine)) {
		return;
	}
	Process_Init(self, info);
	Process_Start(self);
}

/**
 * @brief destructor
 */
void Process_Destroy(Process *self) {
	pthread_cond_destroy(&self->condition);
	pthread_mutex_destroy(&self->mutex);
	RemoveWorkspace(self);
	CLEAR(self);
}

//! @name properties
//! @{

/**
 * @brief Gets whether the process has terminated.
 */
bool Process_HasExited(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return false;
	}
	if (!self->terminated) {
		return false;
	}
	return WIFEXITED(self->status);
}

/**
 * @brief Gets the exit code of the process.
 */
int32_t Process_ExitCode(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return -1;
	}
	if (!self->terminated) {
		return -1;
	}
	return WEXITSTATUS(self->status);
}

double Process_TotalProcessorTime(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return -1;
	}
	if (!self->terminated) {
		return -1;
	}
	return TimevalToDouble(&self->usage.ru_utime) + TimevalToDouble(&self->usage.ru_stime);
}

double Process_UserProcessorTime(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return -1;
	}
	if (!self->terminated) {
		return -1;
	}
	return TimevalToDouble(&self->usage.ru_utime);
}

double Process_StartTime(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return -1;
	}
	return self->startTime;
}

double Process_ExitTime(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return -1;
	}
	return self->exitTime;
}

pid_t Process_Id(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return -1;
	}
	return self->id;
}

FILE *Process_StandardOutput(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return NULL;
	}
	if (self->info.disableOutputCapture) {
		return NULL;
	}
	if (!self->terminated) {
		return NULL;
	}
	char outputPath[64] = { 0 };
	sprintf(outputPath, "%s/" STANDARD_OUTPUT_FILE, self->workspace);
	return fopen(outputPath, "r");
}

FILE *Process_StandardError(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return NULL;
	}
	if (self->info.disableOutputCapture) {
		return NULL;
	}
	if (!self->terminated) {
		return NULL;
	}
	char errorPath[64] = { 0 };
	sprintf(errorPath, "%s/" STANDARD_ERROR_FILE, self->workspace);
	return fopen(errorPath, "r");
}
//! @}

//! @name public methods
//! @{
void Process_Start(Process *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	self->id = fork();
	if (self->id < 0) {
		// error handling
		return;
	}
	if (self->id == 0) {
		char outputPath[64] = { 0 };
		sprintf(outputPath, "%s/" STANDARD_OUTPUT_FILE, self->workspace);
		char errorPath[64] = { 0 };
		sprintf(errorPath, "%s/" STANDARD_ERROR_FILE, self->workspace);
		Execute(&self->info, outputPath, errorPath);	// no return
	}
	self->startTime = (double)GetRealTime() / 1000.0;
	ProcessManager_Register(self->id, self);
}

void Process_Wait(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return;
	}
	pthread_mutex_lock(&self->mutex);
	if (self->terminated) {
		pthread_mutex_unlock(&self->mutex);
		return;
	}
	self->terminated = true;
	pthread_mutex_unlock(&self->mutex);
	wait4(self->id, &self->status, 0, &self->usage);
}

bool Process_WaitFor(Process *self, time_t milliSeconds) {
	if (UNLIKELY(!self || !self->id)) {
		return false;
	}

	time_t limit = GetMonotonicTime() + milliSeconds;
	while (!self->terminated) {
		if (GetMonotonicTime() > limit) {
			return false;
		}
		usleep(1000 * 1);
	}
	return true;
}

void Process_PrintSummary(Process *self) {
	if (UNLIKELY(!self || !self->id)) {
		return;
	}
	if (!Process_HasExited(self)) {
		printf("Process %d is still running\n", self->id);
		return;
	}
	printf("Process %d exited with code %d\n", self->id, Process_ExitCode(self));
	printf("  Total processor time: %.3fs\n", Process_TotalProcessorTime(self));
	printf("  User processor time: %.3fs\n", Process_UserProcessorTime(self));
	printf("  Elapsed time: %.3fs\n", self->exitTime - self->startTime);
}

//! @}

//! @name static methods
//! @{

//! @}



