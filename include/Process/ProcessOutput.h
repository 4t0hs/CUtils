/**
 * @file ProcessOutput.h
 * @brief プロセスの出力
 * @author atohs
 * @date 2024/09/01
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

typedef struct ProcessOutput {
	//! @name public
	//! @{
	size_t size;
	char path[64];
	//! @}
} ProcessOutput;

extern void ProcessOutput_Init(ProcessOutput *self, const char *outputFile);
extern void ProcessOutput_Destroy(ProcessOutput *self);

extern char *ProcessOutput_Read(ProcessOutput *self);
extern ssize_t ProcessOutput_ReadTo(ProcessOutput *self, char *buffer, size_t bufferSize);

#ifdef __cplusplus
}
#endif
