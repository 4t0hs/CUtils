#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "utilities.h"
#include "Stream/MemoryStream.h"
#include <glib-2.0/gio/gio.h>

static off_t GetEndPosition(MemoryStream *self) {
	off_t current = ftello(self->stream);
	if (UNLIKELY(current == -1)) {
		return -1;
	}
	fflush(self->stream);
	fseeko(self->stream, 0, SEEK_END);
	fflush(self->stream);
	off_t end = ftello(self->stream);
	if (UNLIKELY(end == -1)) {
		return -1;
	}
	fseeko(self->stream, current, SEEK_SET);
	return end;
}

static size_t CalculateIncreasingBytes(MemoryStream *self, size_t size) {
	off_t end = GetEndPosition(self);
	if (end == self->position) {
		return size;
	}
	off_t remainingBytes = (off_t)(end - self->position);
	if (remainingBytes < size) {
		return size - remainingBytes;
	}
	return 0;
}

void MemoryStream_Init(MemoryStream *self) {
	CLEAR(self);
	self->stream = open_memstream(&self->buffer, &self->position);
	fflush(self->stream);
	setvbuf(self->stream, NULL, _IOFBF, 512);
	fflush(self->stream);
}

void MemoryStream_Destroy(MemoryStream *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	fclose(self->stream);
	free(self->buffer);
	CLEAR(self);
}

ssize_t MemoryStream_Write(MemoryStream *self, const void *data, size_t size) {
	size_t increasingBytes = CalculateIncreasingBytes(self, size);
	size_t written = fwrite(data, size, 1, self->stream);
	fflush(self->stream);
	if (written != 1) {
		return -errno;
	}
	self->size += increasingBytes;
	return (ssize_t)size;
}

size_t MemoryStream_Read(MemoryStream *self, void *buffer, size_t size) {
	size_t readBytes = fread(buffer, 1, size, self->stream);
	fflush(self->stream);
	return readBytes;
}

int MemoryStream_Seek(MemoryStream *self, off_t offset, int whence) {
	int ret = fseeko(self->stream, offset, whence);
	fflush(self->stream);
	return ret;
}

const void *MemoryStream_RefBuffer(MemoryStream *self) {
	return self->buffer;
}

size_t MemoryStream_GetSize(MemoryStream *self) {
	return self->size;
}
