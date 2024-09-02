/**
 * @file ProcessOutput.c
 * @brief プロセスの出力
 * @author atohs
 * @date 2024/09/01
 */
#include "utilities.h"
#include "Process/ProcessOutput.h"

void ProcessOutput_Init(ProcessOutput *self, const char *outputFile) {
	if (UNLIKELY(!self || !outputFile)) {
		return;
	}
	CLEAR(self);
	self->fp = fopen(outputFile, "r");
	if (!self->fp) {
		return;
	}
	self->size = File_GetSize(outputFile);
}

void ProcessOutput_Destroy(ProcessOutput *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	if (self->fp) {
		fclose(self->fp);
	}
	CLEAR(self);
}

