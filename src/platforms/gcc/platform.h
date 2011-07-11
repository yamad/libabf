#ifndef PLATFORM_H
#define PLATFORM_H

typedef unsigned int UINT;
typedef long int LONGLONG;
typedef int BOOL;

typedef char *LPSTR, *PSTR;
typedef const char *LPCSTR;

typedef void *HANDLE;

/* empty definitions */
#define MEMSET_CTOR
#define STATIC_ASSERT(e)   ((void) 0)
#define ERRORMSG2(msg,a,b) ((void) 0)

#endif
