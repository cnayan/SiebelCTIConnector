#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _WIN32
#include <windows.h>

#else // Any other OS platform - TODO : Limit the support to Windows (and maybe Linux)

#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    extern
#endif

#define APIENTRY __stdcall

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long DWORD;
typedef void* HANDLE;
typedef void *LPVOID;

#define TRUE 1
#define FALSE 0

DECLARE_HANDLE(HINSTANCE);

#endif
