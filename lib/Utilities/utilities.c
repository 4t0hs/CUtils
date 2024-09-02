#include <time.h>
#include "utilities.h"

static inline time_t TimespecToMilliseconds(const struct timespec *time) {
	return (time->tv_sec * 1000) + (time->tv_nsec / 1000000);
}

time_t GetRealTime() {
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return TimespecToMilliseconds(&time);
}

time_t GetMonotonicTime() {
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	return TimespecToMilliseconds(&time);
}
