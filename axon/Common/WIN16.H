/************************************************************************************************
**
**    Copyright (c) 1994 Axon Instruments.
**    All rights reserved.
**
*************************************************************************************************
** HEADER:  WIN16.H
** PURPOSE: Contains standard definitions and prototypes specific to WIN16 systems. 
** AUTHOR:  BHI  May 1994
*/

#ifndef STRICT
   #define STRICT
#endif
#include <windows.h>
#include <windowsx.h>

// Commonly used typedefs & constants.
typedef const void    *LPCVOID;

#ifndef HUGEVAL
#define HUGEVAL  _huge
#endif

// Map WIN16 specific calls to an independent form.
#define GetClassStyle( hwnd )    GetClassWord( (hwnd), GCW_STYLE )
#define GetClassIcon( hwnd )     ((HICON)GetClassWord( (hwnd), GCW_HICON ))
#define SetWindowID( hwnd, nID ) ((int)SetWindowWord( (hwnd), GWW_ID, WORD(nID)))

// Map 32bit calls to 16 bit equivalents.
#define GetWindowThreadProcessId(hwnd, p)    GetWindowTask(hwnd)    // N.B. p unused!
#define GetCurrentThread                     (HANDLE)GetCurrentTask
#define GetCurrentThreadId                   (DWORD)(UINT)GetCurrentTask
#define EnumThreadWindows(dwId,lpfn,lParam)  EnumTaskWindows((HTASK)(int)dwId,lpfn,lParam)

// Symbolic constant for first parameter to GetTempFileName() - BYTE in WIN16, LPSTR in WIN32
#define DEFAULT_DRIVE   0

// DLL entry/exit procs get mapped to the MS WIN32 equivalent.
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH  2
#define DLL_THREAD_DETACH  3
#define DLL_PROCESS_DETACH 0

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

BOOL WINAPI DllMain( HANDLE hDLL, DWORD dwReason, LPVOID lpReserved );

#ifdef __cplusplus
}
#endif

#ifndef EXPORT
#define EXPORT _export
#endif

// WIN16 is by definition a little endian system.
#define LITTLEENDIAN

