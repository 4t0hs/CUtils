#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef int PublishMessageAttribute;
	typedef uint64_t PublishMessage;

	typedef struct PublishContent_t {
		PublishMessage message;
		PublishMessageAttribute attribute;
	} PublishContent_t;

#ifdef __cplusplus
}
#endif
