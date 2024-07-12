/**
 * @file CsvParser.c
 * @brief csvパーサー
 * @author atohs
 * @date 2024/07/12
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Csv/property/CsvProperty.h"
#include "Csv/content/CsvContent.h"
#include "Csv/parser/CsvParser.h"
#include "utilities.h"

/**
 * @brief ファイルバッファ
 */
typedef struct FileBuffer_t {
	char *data;
	size_t size;
} FileBuffer_t;

/**
 * @struct CsvParser_t
 * @brief csvパーサー
 */
struct CsvParser_t {
	CsvProperties_t properties;
	CsvContent_t content;
	FileBuffer_t file;
};

//! ファイルの終端識別子
#define EOF				((uint8_t)0xFF)
//! EOFの長さ
#define EOF_LENGTH		(sizeof(EOF))
//! コンマ
#define COMMA			(',')
//! 改行文字
#define CARRIGE_RETURN	('\r')
//! 改行文字
#define LINE_FEED		('\n')

/**
 * @brief ファイルの終端化判定
 * @param c 文字
 * @return
 */
static inline bool IsEof(char c) {
	return (uint8_t)c == EOF;
}

/**
 * @brief コンマか
 * @param c 文字
 * @return
 */
static inline bool IsComma(char c) {
	return c == COMMA;
}

/**
 * @brief 改行文字か
 * @param c 文字
 * @return
 */
static inline bool IsCr(char c) {
	return c == LINE_FEED;
}

/**
 * @brief 改行文字か
 * @param c 文字
 * @return
 */
static inline bool IsLf(char c) {
	return c == CARRIGE_RETURN;
}

/**
 * @brief 改行文字か
 * @param c 文字
 * @param nextChar 次の文字
 * @return
 */
static inline bool IsCrLf(char c, char nextChar) {
	return IsLf(c) && IsCr(nextChar);
}

/**
 * @brief 文字を終端する
 * @param c 文字
 */
static inline void Terminate(char *c) {
	*c = '\0';
}

/**
 * @brief 不要な文字か
 * @param c 文字
 * @return
 */
static inline bool IsExtraChars(char c) {
	return c == '\0' || IsCr(c) || IsLf(c);
}

/**
 * @brief アスキー文字か
 * @param c
 * @return
 */
static inline bool IsAscii(char c) {
	return (uint8_t)c <= 0x7F;
}

/**
 * @brief 有効な文字コードか
 * @param data データ
 * @param size サイズ
 * @return
 */
static bool IsNotSupportedCharCode(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (!IsAscii(data[i]) && !IsEof(data[i])) {
			return true;
		}
	}
	return false;
}

/**
 * @brief EOFで埋める
 * @param data データ
 * @param size サイズ
 */
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

/**
 * @brief パース
 * @param self インスタンス
 * @return 結果
 */
static CsvReturnCode Parse(CsvParser_t *self) {
	if (IsNotSupportedCharCode(self->file.data, self->file.size)) {
		return CSV_INVALID_CHAR_CODE;
	}
	CsvLine_t line;
	CsvItem_t item;
	CsvLine_Init(&line);
	CsvItem_Init(&item);
	CsvItem_Set(&item, self->file.data);

	for (char *c = self->file.data; true; c++) {
		if (IsEof(*c)) {
			Terminate(c);
			CsvLine_MoveBackItem(&line, &item);
			CsvContent_MoveBackLine(&self->content, &line);
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
			CsvContent_MoveBackLine(&self->content, &line);
			CsvLine_Init(&line);
			CsvItem_Set(&item, nextChar + 1);
		} else if (IsCr(*c)) {
			Terminate(c);
			CsvLine_MoveBackItem(&line, &item);
			CsvContent_MoveBackLine(&self->content, &line);
			CsvLine_Init(&line);
			CsvItem_Set(&item, nextChar);
		} else {
			// do nothing
		}
	}
	return CSV_SUCCESS;
}

/**
 * @brief ファイル読込み
 * @param self インスタンス
 * @param filePath ファイルパス
 * @return 0=成功
 */
static int LoadFile(CsvParser_t *self, const char *filePath) {
	ssize_t file_size = File_GetSize(filePath);
	if (file_size < 0) {
		return (int)file_size;
	}
	size_t allocate_size = file_size + EOF_LENGTH;
	self->file.data = malloc(allocate_size);
	int ret = File_Read(filePath, self->file.data, file_size);
	if (ret < 0) {
		free(self->file.data);
		self->file.data = NULL;
		return ret;
	}
	self->file.size = allocate_size;
	FillEof(self->file.data, self->file.size);
	return 0;
}

/**
 * @brief 初期化
 * @param props プロパティ
 * @return インスタンス
 */
CsvParser_t *CsvParser_Init(const CsvProperties_t *props) {
	CsvParser_t *self = (CsvParser_t *)calloc(1, sizeof(*self));

	self->properties = *props;
	CsvContent_Init(&self->content);
	return self;
}

/**
 * @brief データを読み込んでパースする
 * @param self インスタンス
 * @param filePath ファイルパス
 * @return ステータス
 */
CsvReturnCode CsvParser_LoadFromFile(CsvParser_t *self, const char *filePath) {
	if (UNLIKELY(!self)) {
		return CSV_INVALID_PARAMETER;
	}
	if (LoadFile(self, filePath) != 0) {
		return CSV_INVALID_PARAMETER;
	}
	return Parse(self);
}

/**
 * @brief データを読み込んでパースする
 * @param self インスタンス
 * @param data データ
 * @param dataSize データサイズ
 * @return ステータス
 */
CsvReturnCode CsvParser_LoadFromData(CsvParser_t *self, const char *data, size_t dataSize) {
	if (UNLIKELY(!self || !data || dataSize == 0)) {
		return CSV_INVALID_PARAMETER;
	}
	size_t allocate_size = dataSize + EOF_LENGTH;
	self->file.data = malloc(allocate_size);
	self->file.size = allocate_size;
	memcpy(self->file.data, data, dataSize);
	FillEof(self->file.data, allocate_size);
	return Parse(self);
}

/**
 * @brief csvデータを取得する
 * @param self インスタンス
 * @return 参照
 */
const CsvContent_t *CsvParser_GetContent(CsvParser_t *self) {
	if (UNLIKELY(!self)) {
		return NULL;
	}
	return &self->content;
}

/**
 * @brief インスタンスを破棄
 * @param self インスタンス
 */
void CsvParser_Destroy(CsvParser_t *self) {
	if (UNLIKELY(!self)) {
		return;
	}
	CsvContent_Destroy(&self->content);
	if (self->file.data) free(self->file.data);
	free(self);
	self = NULL;
}


