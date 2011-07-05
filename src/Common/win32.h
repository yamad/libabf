/************************************************************************************************
**
**    Copyright (c) 1994-1997 Axon Instruments.
**    All rights reserved.
**
*************************************************************************************************
** HEADER:  win32.h
** PURPOSE: Contains standard definitions and prototypes specific to WIN32 systems. 
** AUTHOR:  BHI  May 1994
*/
#ifndef INC_WIN32_H
#define INC_WIN32_H

#ifndef STRICT
   #define STRICT
#endif
#pragma warning(disable:4201)   // warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable:4514)   // warning C4514: 'IsEqualGUID' : unreferenced inline function has been removed

#include <windows.h>
#include <windowsx.h>

//==============================================================================================
// The following macro wraps the uglyness of the declspec declaration to prevent a
// VTable being allocated for a pure virtual class definition.
//
#ifndef PURE_INTERFACE
   #ifdef _MSC_VER
      #define PURE_INTERFACE _declspec(novtable)
   #else
      #define PURE_INTERFACE
   #endif
#endif

//=======================================================================================
// Oversights that should have been in the WINDOWS.H headers.
//
#ifndef LPCPOINT
typedef const POINT *LPCPOINT;
#endif

//==============================================================================================
// The following inline functions implement calls that should have been in WINDOWSX.H.
//
//==============================================================================================
// FUNCTION: GetClassStyle
// PURPOSE:  Returns the class style bits for the window class associated with a window handle.
//
inline DWORD GetClassStyle(HWND hWnd)
{
   return (DWORD)GetClassLong( hWnd, GCL_STYLE );
}

//==============================================================================================
// FUNCTION: GetClassIcon
// PURPOSE:  Returns the HICON for the window class associated with a window handle.
//
inline HICON GetClassIcon(HWND hWnd)
{
   return (HICON)GetClassLong( hWnd, GCL_HICON );
}

//==============================================================================================
// FUNCTION: SetWindowID
// PURPOSE:  Sets the ID associated with a window handle.
//
inline int SetWindowID(HWND hWnd, int nID)
{
   return (int)SetWindowLong( hWnd, GWL_ID, nID );
}

//==============================================================================================
// FUNCTION: SetWindowStyle
// PURPOSE:  Sets the window style bits for a given window.
//
inline DWORD SetWindowStyle(HWND hWnd, DWORD dwStyle)
{
   return (DWORD)SetWindowLong( hWnd, GWL_STYLE, long(dwStyle) );
}

//==============================================================================================
// FUNCTION: SetWindowExStyle
// PURPOSE:  Sets the window style bits for a given window.
//
inline DWORD SetWindowExStyle(HWND hWnd, DWORD dwStyle)
{
   return (DWORD)SetWindowLong( hWnd, GWL_EXSTYLE, long(dwStyle) );
}

//===============================================================================================
// Message crackers left out of WINDOWSX.H

#ifndef HANDLE_WM_CAPTURECHANGED

/* void Cls_OnCaptureChanged(HWND hwnd, HWND hwndNewCapture) */
#define HANDLE_WM_CAPTURECHANGED(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (HWND)(lParam)), 0L)
#define FORWARD_WM_CAPTURECHANGED(hwnd, hwndNewCapture, fn) \
    (void)(fn)((hwnd), WM_CAPTURECHANGED, 0, (LPARAM)(HWND)(hwndNewCapture))

#endif   // HANDLE_WM_CAPTURECHANGED

#if(_WIN32_WINNT >= 0x0400)
#ifndef HANDLE_WM_MOUSEWHEEL

/* void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys) */
#define HANDLE_WM_MOUSEWHEEL(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)))
#define FORWARD_WM_MOUSEWHEEL(hwnd, xPos, yPos, zDelta, fwKeys, fn) \
    (void)(fn)((hwnd), WM_MOUSEWHEEL, MAKEWPARAM((fwKeys),(zDelta)), MAKELPARAM((x),(y)))

#endif   // HANDLE_WM_MOUSEWHEEL
#endif   // (_WIN32_WINNT >= 0x0400)

#ifndef HANDLE_WM_ENTERSIZEMOVE

/* void Cls_OnSizeMove(HWND hWnd, BOOL bEnter) */
#define HANDLE_WM_ENTERSIZEMOVE(hwnd, wParam, lParam, fn)   \
    ((fn)((hwnd), TRUE), 0L)
#define FORWARD_WM_ENTERSIZEMOVE(hwnd, fn)  \
    (void)(fn)((hwnd), WM_ENTERSIZEMOVE, 0, 0)

/* void Cls_OnSizeMove(HWND hWnd, BOOL bEnter) */
#define HANDLE_WM_EXITSIZEMOVE(hwnd, wParam, lParam, fn)    \
    ((fn)((hwnd), FALSE), 0L)
#define FORWARD_WM_EXITSIZEMOVE(hwnd, fn)   \
    (void)(fn)((hwnd), WM_EXITSIZEMOVE, 0, 0)

#endif   // HANDLE_WM_ENTERSIZEMOVE

// Symbolic constant for first parameter to GetTempFileName() - BYTE in WIN16, LPSTR in WIN32
#define DEFAULT_DRIVE   "."

// Error return from SetFilePointer()
#ifndef INVALID_SEEK_VALUE
#define INVALID_SEEK_VALUE    (0xFFFFFFFF)
#endif

#ifndef EXPORT
#define EXPORT
#endif

#ifndef HUGEVAL
#define HUGEVAL
#endif

#ifndef _M_IX86
#error "_M_IX86 not defined. Check compiler settings."
#endif

#endif   // INC_WIN32_H
