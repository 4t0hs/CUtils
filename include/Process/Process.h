/**
 * @file Process.h
 * @brief プロセス
 * @author atohs
 * @date 2024/08/29
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/resource.h>
#include <stdio.h>
	// #include <stdatomic.h>
#include "Process/ProcessOutput.h"

	/**
	 * @brief Information to start a process
	 */
	typedef struct ProcessStartInfo {
		/**
		 * @brief Command line to execute
		 */
		const char *commandLine;
		/**
		 * @brief Environment variables
		 */
		char *const *environment;

		bool disableOutputCapture;
	} ProcessStartInfo;

	/**
	 * @brief A process
	 */
	typedef struct Process {
		//! @name private
		//! @{
		ProcessStartInfo info;
		int32_t status;
		pthread_mutex_t mutex;
		pthread_cond_t condition;
		bool isWaiting;
		struct rusage usage;
		char workspace[32];
		pid_t id;
		struct {
			void *address;
			size_t size;
		} stack;
		double startTime;
		double exitTime;
		bool terminated;
		//! @}

		//! @name public
		//! @{
		void (*onExited)(struct Process *self);
		//! @}
	} Process;

	extern void Process_Init(Process *self, const ProcessStartInfo *info);
	extern void Process_Destroy(Process *self);

	extern void Process_Execute(Process *self, const ProcessStartInfo *info);

	extern bool Process_HasExited(Process *self);
	extern int32_t Process_ExitCode(Process *self);
	extern double Process_TotalProcessorTime(Process *self);
	extern double Process_UserProcessorTime(Process *self);
	extern double Process_StartTime(Process *self);
	extern double Process_ExitTime(Process *self);
	extern pid_t Process_Id(Process *self);
	extern void Process_StandardOutput(Process *self, ProcessOutput *result);
	extern void Process_StandardError(Process *self, ProcessOutput *result);

	extern void Process_Start(Process *self);
	extern void Process_Wait(Process *self);
	extern bool Process_WaitFor(Process *self, time_t milliSeconds);

	extern void Process_PrintSummary(Process *self);

#ifdef __cplusplus
}
#endif
