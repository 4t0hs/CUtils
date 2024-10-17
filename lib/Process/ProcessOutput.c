/**
 * @file ProcessOutput.c
 * @brief プロセスの出力
 * @author atohs
 * @date 2024/09/01
 */
#include <stdlib.h>
#include "utilities.h"
#include "Process/ProcessOutput.h"

void ProcessOutput_Init(ProcessOutput *self, const char *outputFile) {
	if (UNLIKELY(!self || !outputFile)) {
		return;
	}
	CLEAR(self);
	strncpy(self->path, outputFile, sizeof(self->path));
	self->size = File_GetSize(outputFile);
}

void ProcessOutput_Destroy(ProcessOutput *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	CLEAR(self);
}


char *ProcessOutput_Read(ProcessOutput *self) {
	if (UNLIKELY(!self || !self->size)) {
		return NULL;
	}
	FILE *fp = fopen(self->path, "r");
	if (!fp) {
		return NULL;
	}
	char *buffer = calloc(1, self->size + 1);
	if (buffer) {
		fread(buffer, self->size, 1, fp);
	}
	fclose(fp);
	return buffer;
}

ssize_t ProcessOutput_ReadTo(ProcessOutput *self, char *buffer, size_t bufferSize) {
	if (UNLIKELY(!self || !self->size)) {
		return -1;
	}
	FILE *fp = fopen(self->path, "r");
	if (!fp) {
		return -1;
	}
	size_t size = fread(buffer, 1, bufferSize > self->size ? self->size : bufferSize, fp);
	fclose(fp);
	return (ssize_t)size;
}
