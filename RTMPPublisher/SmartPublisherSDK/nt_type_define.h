/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#ifndef NT_TYPE_DEFINE_H_
#define NT_TYPE_DEFINE_H_

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
/*#define NT_API __cdecl*/
#define NT_API __stdcall
#define NT_CALLBACK __stdcall
#else
#define NT_API
#define NT_CALLBACK
#endif

typedef unsigned char		NT_UINT8;
typedef NT_UINT8			NT_BYTE;
typedef char				NT_CHAR;
typedef short				NT_INT16;
typedef unsigned short		NT_UINT16;
typedef int					NT_INT32;
typedef unsigned int		NT_UINT32;
typedef unsigned long long	NT_UINT64;
typedef long long           NT_INT64;
//typedef void*				NT_PVOID;

typedef NT_BYTE*			NT_PBYTE;
typedef NT_BYTE const*		NT_PCBYTE;

typedef NT_CHAR*			NT_PSTR;
typedef NT_CHAR const*		NT_PCSTR;


#define NT_VOID void

typedef void* NT_PVOID;
typedef void* NT_HANDLE;
typedef NT_HANDLE * NT_PHANDLE;


#ifdef __cplusplus
}
#endif

#endif