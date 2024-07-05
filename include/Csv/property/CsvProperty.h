#pragma once

#include <stdbool.h>

typedef enum CsvEolCode {
	CSV_EOL_CR = 0,
	CSV_EOL_CRLF,
} CsvEolCode;

typedef struct CsvProperties {
	bool		has_header;
} CsvProperties_t;
