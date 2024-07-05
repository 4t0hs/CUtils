#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Csv/property/CsvProperty.h"
#include "Csv/content/CsvContent.h"
#include "Csv/parser/CsvParser.h"
#include "utilities.h"

typedef struct FileBuffer {
	char *data;
	size_t size;
} FileBuffer_t;
struct CsvParser {
	CsvProperties_t properties;
	CsvContent_t content;
	FileBuffer_t file;
};

#define EOF				((uint8_t)0xFF)
#define EOF_LENGTH		(sizeof(EOF))
#define COMMA			(',')
#define CARRIGE_RETURN	('\r')
#define LINE_FEED		('\n')
static inline bool IsEof(char c) {
	return (uint8_t)c == EOF;
}
static inline bool IsComma(char c) {
	return c == COMMA;
}
static inline bool IsCr(char c) {
	return c == LINE_FEED;
}
static inline bool IsLf(char c) {
	return c == CARRIGE_RETURN;
}
static inline bool IsCrLf(char c, char nextChar) {
	return IsLf(c) && IsCr(nextChar);
}

static inline void Terminate(char *c) {
	*c = '\0';
}

static inline bool IsExtraChars(char c) {
	return c == '\0' || IsCr(c) || IsLf(c);
}

static inline bool IsAscii(char c) {
	return (uint8_t)c <= 0x7F;
}

static bool IsNotSupportedCharCode(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (!IsAscii(data[i]) && !IsEof(data[i])) {
			return true;
		}
	}
	return false;
}

static void FillEof(char *data, size_t size) {
	// 最後の値は必ずEofにする
	data[size - 1] = EOF;
	for (size_t i = size - 2; i != 0; i--) {
		if (!IsExtraChars(data[i])) {
			return;
		}
		data[i] = EOF;
	}
}

static CsvReturnCode Parse(CsvParser_t *this) {
	if (IsNotSupportedCharCode(this->file.data, this->file.size)) {
		return CSV_INVALID_CHAR_CODE;
	}
	CsvLine_t line;
	CsvItem_t item;
	CsvLine_Init(&line);
	CsvItem_Init(&item);
	CsvItem_Set(&item, this->file.data);

	for (char *c = this->file.data; true; c++) {
		if (IsEof(*c)) {
			Terminate(c);
			CsvLine_MoveBackItem(&line, &item);
			CsvContent_MoveBackLine(&this->content, &line);
			break;
		}
		char *nextChar = c + 1;
		if (IsComma(*c)) {
			Terminate(c);	// 終端
			CsvLine_MoveBackItem(&line, &item);
			CsvItem_Set(&item, nextChar);
		} else if (IsCrLf(*c, *nextChar)) {
			Terminate(c);
			Terminate(nextChar);
			CsvLine_MoveBackItem(&line, &item);
			CsvContent_MoveBackLine(&this->content, &line);
			CsvLine_Init(&line);
			CsvItem_Set(&item, nextChar + 1);
		} else if (IsCr(*c)) {
			Terminate(c);
			CsvLine_MoveBackItem(&line, &item);
			CsvContent_MoveBackLine(&this->content, &line);
			CsvLine_Init(&line);
			CsvItem_Set(&item, nextChar);
		} else {
			// do nothing
		}
	}
	return CSV_SUCCESS;
}

static int load_file(CsvParser_t *this, const char *filePath) {
	ssize_t file_size = File_GetSize(filePath);
	if (file_size < 0) {
		return (int)file_size;
	}
	size_t allocate_size = file_size + EOF_LENGTH;
	this->file.data = malloc(allocate_size);
	int ret = File_Read(filePath, this->file.data, file_size);
	if (ret < 0) {
		free(this->file.data);
		this->file.data = NULL;
		return ret;
	}
	this->file.size = allocate_size;
	FillEof(this->file.data, this->file.size);
	return 0;
}

CsvParser_t *CsvParser_Init(const CsvProperties_t *props) {
	CsvParser_t *this = (CsvParser_t *)calloc(1, sizeof(*this));

	this->properties = *props;
	CsvContent_Init(&this->content);
	return this;
}

CsvReturnCode CsvParser_LoadFromFile(CsvParser_t *this, const char *filePath) {
	if (!this) return CSV_INVALID_PARAMETER;
	if (load_file(this, filePath) != 0) {
		return CSV_INVALID_PARAMETER;
	}
	return Parse(this);
}

CsvReturnCode CsvParser_LoadFromData(CsvParser_t *this, const char *data, size_t dataSize) {
	if (!this) return CSV_INVALID_PARAMETER;
	if (!data || dataSize == 0) {
		return CSV_INVALID_PARAMETER;
	}
	size_t allocate_size = dataSize + EOF_LENGTH;
	this->file.data = malloc(allocate_size);
	this->file.size = allocate_size;
	memcpy(this->file.data, data, dataSize);
	FillEof(this->file.data, allocate_size);
	return Parse(this);
}

const CsvContent_t *CsvParser_GetContent(CsvParser_t *this) {
	if (!this) return NULL;
	return &this->content;
}

void CsvParser_Destroy(CsvParser_t *this) {
	if (!this) return;

	CsvContent_Destroy(&this->content);
	if (this->file.data) free(this->file.data);
	free(this);
	this = NULL;
}


