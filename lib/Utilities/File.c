/**
 * @file File.c
 * @brief ファイル操作
 * @author atohs
 * @date 2024/07/12
 */
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "File.h"

/**
 * @brief ファイルサイズ取得
 * @param path
 * @return 0 < error code
 */
ssize_t File_GetSize(const char *path) {
	struct stat st = { 0 };
	if (stat(path, &st) != 0) {
		return -errno;
	}
	return st.st_size;
}

/**
 * @brief ファイルの存在確認
 * @param path 
 * @return 
 */
bool File_Exists(const char *path) {
	struct stat st = { 0 };
	return stat(path, &st) ? false : true;
}

/**
 * @brief ファイル読込み
 * @param path 
 * @param buffer 
 * @param size 
 * @return 読込みサイズ
 */
int File_Read(const char *path, void *buffer, size_t size) {
	FILE *fp = fopen(path, "r");
	if (!fp) return -errno;
	if (fread(buffer, size, 1, fp) != size) {
		fclose(fp);
		return -errno;
	}
	fclose(fp);
	return 0;
}

int File_Create(const char *path) {
	FILE *fp = fopen(path, "w");
	if (!fp) return -errno;
	fclose(fp);
	return 0;
}
