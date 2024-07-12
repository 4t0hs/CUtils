/**
 * @file PublishContent.h
 * @brief パブリッシュの内容
 * @author atohs
 * @date 2024/07/12
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
	//! メッセージの属性
	typedef int PublishMessageAttribute;
	//! メッセージ
	typedef uint64_t PublishMessage;

	/**
	 * @brief パブリッシュの内容
	 */
	typedef struct PublishContent_t {
		//! メッセージ
		PublishMessage message;
		//! 属性
		PublishMessageAttribute attribute;
	} PublishContent_t;

#ifdef __cplusplus
}
#endif
