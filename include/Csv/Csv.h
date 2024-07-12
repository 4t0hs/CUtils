/**
 * @file Csv.h
 * @brief
 * @author atohs
 * @date 2024/07/12
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief ステータス
	 */
	typedef enum CsvReturnCode {
		//! 成功
		CSV_SUCCESS = 0,
		//! 無効な引数
		CSV_INVALID_PARAMETER,
		//! 無効な文字コード
		CSV_INVALID_CHAR_CODE,
	} CsvReturnCode;

#ifdef __cplusplus
}
#endif
