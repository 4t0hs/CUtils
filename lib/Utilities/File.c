#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "File.h"

ssize_t File_GetSize(const char *path) {
	struct stat st = { 0 };
	if (stat(path, &st) != 0) {
		return -errno;
	}
	return st.st_size;
}

bool File_Exists(const char *path) {
	struct stat st = { 0 };
	return stat(path, &st) ? false : true;
}

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
