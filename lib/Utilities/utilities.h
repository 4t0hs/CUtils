#pragma once

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "File.h"

#define CLEAR(objectPointer)	memset(objectPointer, 0, sizeof(*objectPointer))

#define LIKELY(condition)		__glibc_likely(!!(condition))
#define UNLIKELY(condition)		__glibc_unlikely(!!(condition))

#ifdef __cplusplus
}
#endif

