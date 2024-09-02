/**
 * @file File.h
 * @brief ファイル操作
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <stdbool.h>

	ssize_t File_GetSize(const char *path);
	bool File_Exists(const char *path);
	int File_Read(const char *path, void *buffer, size_t size);
	int File_Create(const char *path);

#ifdef __cplusplus
}
#endif



