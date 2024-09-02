/**
 * @file utilities.h
 * @brief ユーティリティ
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "File.h"

#define CLEAR(objectPointer)	memset(objectPointer, 0, sizeof(*objectPointer))

#define LIKELY(condition)		__glibc_likely(!!(condition))
#define UNLIKELY(condition)		__glibc_unlikely(!!(condition))

time_t GetRealTime();
time_t GetMonotonicTime();

#ifdef __cplusplus
}
#endif

