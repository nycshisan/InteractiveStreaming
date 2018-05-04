/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#ifndef SMART_LOG_DEFINE_H_
#define SMART_LOG_DEFINE_H_

#include "nt_type_define.h"
#include "nt_base_code_define.h"

#ifdef __cplusplus
extern "C"{
#endif
	/* 错误码 */
	typedef enum _SL_E_ERROR_CODE
	{
		NT_ERC_SL_LEVEL_ERROR = (NT_ERC_SMART_LOG | 0x1),
	} SL_E_ERROR_CODE;

	/* 设置参数ID, 这个目前这么写 */
	typedef enum _SL_E_PARAM_ID
	{
		SL_PARAM_ID_BASE = NT_PARAM_ID_SMART_LOG,
	} SL_E_PARAM_ID;

	typedef enum _SL_LOG_LEVEL
	{
		SL_TRACE_LEVEL,
		SL_DEBUG_LEVEL,
		SL_INFO_LEVEL,
		SL_WARNING_LEVEL,
		SL_ERROR_LEVEL,
		SL_FATAL_LEVEL,
		SL_CLOSE_LEVEL,

	} SL_LOG_LEVEL;

#ifdef __cplusplus
}
#endif

#endif