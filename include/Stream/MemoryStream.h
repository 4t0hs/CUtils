/**
 * @file MemoryStream.h
 * @brief MemoryStream
 * @author atohs
 * @date 2024/11/05
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct MemoryStream {
	FILE *stream;
	char *buffer;
	size_t position;
	size_t size;
} MemoryStream;

void MemoryStream_Init(MemoryStream *self);

void MemoryStream_Destroy(MemoryStream *self);

ssize_t MemoryStream_Write(MemoryStream *self, const void *data, size_t size);

size_t MemoryStream_Read(MemoryStream *self, void *buffer, size_t size);

int MemoryStream_Seek(MemoryStream *self, off_t offset, int whence);

const void *MemoryStream_RefBuffer(MemoryStream *self);

size_t MemoryStream_GetSize(MemoryStream *self);

#ifdef __cplusplus
}
#endif

