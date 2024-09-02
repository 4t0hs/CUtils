#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <stdint.h>

typedef void (*ProcessExitedEventHandler)(pid_t pid, int32_t exitStatus, void *data);

void ProcessManager_Init(ProcessExitedEventHandler onExited);

void ProcessManager_Destroy();

void ProcessManager_Register(pid_t pid, void *data);

void ProcessManager_Unregister(pid_t pid);

#ifdef __cplusplus
}
#endif

