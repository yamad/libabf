#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <unistd.h>

/* Define macros to identify the version of C being used */
#if defined(__STDC__)
# define C89
# if defined(__STDC_VERSION__)
#  define C90
#  if (__STDC_VERSION__ >= 199409L)
#   define C94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C99
#  endif
# endif
#endif

/* If not using C99, create boolean macros as in stdbool ourselves */
#if C99
#include <stdbool.h>
#else
#ifndef __bool_true_false_are_defined
#define bool int8_t
#define true 1
#define false 0
#define __bool_true_false_are_defined 1
#endif
#endif

/* Ensure that booleans in filedata are 8-bit */
typedef int8_t t_BOOL;

typedef int32_t INT;
typedef uint32_t UINT;

typedef uint8_t CHAR;
typedef uint16_t USHORT;
typedef int16_t SHORT;
typedef uint32_t ULONG;
typedef int32_t LONG;
typedef int64_t LONGLONG;

typedef uint16_t WORD;
typedef uint32_t DWORD;

typedef float FLOAT;

typedef char *LPSTR, *PSTR;
typedef const char *LPCSTR;

typedef void *HANDLE;

/* empty definitions */
#define MEMSET_CTOR
#define STATIC_ASSERT(x)
#define ERRORMSG2(msg,a,b)

/* GUID is normally defined in the Windows Platform SDK */
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID;
#endif /* GUID_DEFINED */

#endif
