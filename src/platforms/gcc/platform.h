#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

typedef int BOOL;
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

#endif
