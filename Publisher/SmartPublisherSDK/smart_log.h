/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#ifndef SMART_LOG_H_ 
#define SMART_LOG_H_

#include "nt_type_define.h"
#include "smart_log_define.h"

#ifdef __cplusplus
extern "C"{
#endif

	typedef struct _SmartLogAPI
	{
		/*
		设置日志目录，Windows平台请设置宽字符，比如L"D:\\xxx\gg"
		*/
		NT_UINT32 (NT_API *SetPath)(NT_PVOID path);

		/*
		设置日志等级，level请参考SL_LOG_LEVEL
		*/
		NT_UINT32 (NT_API *SetLevel)(NT_UINT32 level);

		/*
		将缓冲全部写入
		*/
		NT_UINT32(NT_API* Flush)();

		/*
		万能接口, 设置参数， 大多数问题， 这些接口都能解决
		*/
		NT_UINT32(NT_API *SetParam)(NT_UINT32 id, NT_PVOID pData);

		/*
		万能接口, 得到参数， 大多数问题，这些接口都能解决
		*/
		NT_UINT32(NT_API *GetParam)(NT_UINT32 id, NT_PVOID pData);

	} SmartLogAPI;


	NT_UINT32 NT_API GetSmartLogAPI(SmartLogAPI* pAPI);

#ifdef __cplusplus
}
#endif

#endif
