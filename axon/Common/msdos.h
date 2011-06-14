/************************************************************************************************
**
**    Copyright (c) 1994 Axon Instruments.
**    All rights reserved.
**
*************************************************************************************************
** HEADER:  msdos.h
** PURPOSE: Contains standard definitions and prototypes specific to MSDOS systems. 
** AUTHOR:  BHI  May 1994
*/

//
// Commonly used typedefs & constants from windows.h.
//
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned long *LPDWORD;
typedef unsigned int   UINT;
typedef int            BOOL;
typedef long           LONG;
typedef long          *PLONG;
typedef char          *LPSTR;
typedef unsigned char *LPBYTE;
typedef const char    *LPCSTR;
typedef void          *LPVOID;
typedef const void    *LPCVOID;

#define PASCAL         _pascal
#define CDECL          _cdecl

#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
#define MAKELP(sel, off)    ((void FAR*)MAKELONG((off), (sel)))

#define TRUE  1
#define FALSE 0

#define HUGEVAL _huge
#define FAR     _far
#define NEAR    _near

#define EXPORT
#define LITTLEENDIAN

// Handle declarations.
typedef const void NEAR*        HANDLE;
typedef HANDLE HINSTANCE;

#define DECLARE_HANDLE(name)    struct name##__ { int unused; }; \
                                typedef const struct name##__ NEAR* name
#define DECLARE_HANDLE32(name)  struct name##__ { int unused; }; \
                                typedef const struct name##__ FAR* name
                                
#define CALLBACK AXOAPI                                
                                
#if 0         // remove mapping of WIN16 functions in favor of WIN32 versions in WIN32FIO.H
//
// MACRO wrappers to map Windows low level file I/O functions to C runtime equivalents for
// non Windows compilations.
//

#include <stdio.h>               // SEEK_SET, remove
#include <fcntl.h>               // _O_RDWR, _O_BINARY etc
#include <io.h>                  // open, close, read, write, lseek, creat
#include <sys\stat.h>            // _S_IWRITE

typedef int HFILE;
#define HFILE_ERROR ((HFILE)(-1))
#define _lopen(szFilename, uFlag)   _open(szFilename, _O_BINARY | (uFlag))
#define _lcreat(szFilename, uFlag)  _creat(szFilename, _S_IWRITE | _S_IREAD)
#define _lclose(h)                  _close(h)
#define _llseek(h, p, f)            _lseek(h, p, f)
#define _lread(h, p, c)             (UINT)_read(h, p, c)
#define _lwrite(h, p, c)            (UINT)_write(h, p, c)

#define OF_READ             _O_RDONLY
#define OF_WRITE            _O_WRONLY
#define OF_READWRITE        _O_RDWR
#define OF_SHARE_COMPAT     0
#define OF_SHARE_EXCLUSIVE  0
#define OF_SHARE_DENY_WRITE 0
#define OF_SHARE_DENY_READ  0
#define OF_SHARE_DENY_NONE  0

#endif

// Compatibility #defines for WIN16/WIN32 functions under DOS.
#define DEFAULT_DRIVE   "."
#define GetTempFileName(a,b,c,d) strlen(tmpnam(d))


