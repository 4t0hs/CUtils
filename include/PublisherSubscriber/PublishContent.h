#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

	typedef int PublishMessageAttribute;
	typedef uint64_t PublishMessage;

	typedef struct PublishContent_t {
		PublishMessage message;
		PublishMessageAttribute attribute;
	} PublishContent_t;

#ifdef __cplusplus
}
#endif
