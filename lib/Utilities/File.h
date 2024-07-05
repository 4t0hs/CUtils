#pragma once

#include <unistd.h>
#include <stdbool.h>

ssize_t File_GetSize(const char *path);
bool File_Exists(const char *path);
int File_Read(const char *path, void *buffer, size_t size);

