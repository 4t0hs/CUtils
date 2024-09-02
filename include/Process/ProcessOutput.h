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
	FILE *fp;
	size_t size;
	//! @}
} ProcessOutput;

extern void ProcessOutput_Init(ProcessOutput *self, const char *outputFile);
extern void ProcessOutput_Destroy(ProcessOutput *self);

#ifdef __cplusplus
}
#endif
