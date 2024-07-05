#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef enum CsvReturnCode {
	CSV_SUCCESS = 0,
	CSV_INVALID_PARAMETER,
	CSV_INVALID_CHAR_CODE,
} CsvReturnCode;

#ifdef __cplusplus
}
#endif
