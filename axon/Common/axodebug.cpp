//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  axodebug.cpp
// PURPOSE: Contains utility macros and defines for Axon development debugging.
// AUTHOR:  BHI  Oct 1993
//

#include "wincpp.hpp"
#include <stdarg.h>
#include <io.h>
#include <stdlib.h>
#include "..\common\axodebug.h"

#define AXOTRACE_UNTESTED  HWND(-1)

static HWND s_hwndDebugWin      = AXOTRACE_UNTESTED;
static char s_szTraceWinClass[] = "AxonTraceWindow";
static char s_szTraceWinTitle[] = "AxoTrace";
static DWORD s_dwAxoDebugFlags  = 0;

#define  TRACE_BUFFER_LENGTH  2048
#define  TRACE_PREFIX_LENGTH  1024

static char  s_szTracePrefix[TRACE_PREFIX_LENGTH] = {0};

#define AXOTRACE_SIGNATURE (*PDWORD(LPVOID("BHI ")))

//*********************************************************************************************
// FUNCTION: get_module_filename
// PURPOSE:  Helper function to get the long filename from the given module handle.
//
static void get_module_filename(HMODULE handle, char * filename)
{
   // If this was not debugging code, I would do this:
   // WARRAYASSERT(filename, (_MAX_FNAME+_MAX_EXT));
   
   char short_path[_MAX_PATH] = {0};
   if (::GetModuleFileName(handle, short_path, sizeof(short_path)))
   {
      // According to the documentation:
      // Under Win95, GetModuleFileName() returns short filenames if your APPs version is < 4.00
      // Unfortunately, GetLongPathName() does not exist in NT4 or 95, and the work-arounds involve
      // lot's of GetProcAddress() and COM functions.
      // Given that this is only for internal trace messages, we can live with the short filename.
      char  fname [_MAX_FNAME];
      char  ext   [_MAX_EXT];
      _splitpath(short_path, NULL, NULL, fname, ext);
      _makepath(filename, NULL, NULL, fname, ext);
   }
}

//==============================================================================================
// FUNCTION: _ShowTraceString
// PURPOSE:  Show a debugging message (if possible) and put up a message box for assertion failures.
//
static void _ShowTraceString( LPCTSTR lpsz )
{
#if 0
   FILE *h = fopen("\\Axon.log", "a+");
   fprintf(h, "%s\n", lpsz);
   fflush(h);
   _commit(_fileno(h));
   fclose(h);
#endif

   if (s_hwndDebugWin==AXOTRACE_UNTESTED)
      s_hwndDebugWin = FindWindow(s_szTraceWinClass, s_szTraceWinTitle);
      
   // Output the string with OutputDebugString() only if there is NOT a target window to receive it.      
   if (!s_hwndDebugWin)
   {
      OutputDebugString( lpsz );
      return;
   }

   COPYDATASTRUCT cds;
   memset(&cds, 0, sizeof(cds));
   cds.dwData = AXOTRACE_SIGNATURE;
   cds.cbData = strlen(lpsz)+1;
   cds.lpData = (void *)lpsz;
   SendMessage(s_hwndDebugWin, WM_COPYDATA, NULL, LPARAM(&cds));
}

//==============================================================================================
// FUNCTION: ReportHookFn
// PURPOSE:  Callback function that gets hooked into the DEBUG reporting by AXODBG_Initialize.
//
static int ReportHookFn(int nReportType, char *pszMessage, int *pnReturnValue)
{
   _ShowTraceString( pszMessage );

   // values defined in <CRTDBG.H>
   switch (nReportType)
   {
   case _CRT_WARN:
      // Don't break on trace messages from within the CRT
      break;
   case _CRT_ERROR:
   case _CRT_ASSERT:
   case _CRT_ERRCNT:
      // If we are breaking immediately, return TRUE with a "ReturnValue" of 1.
      if (s_dwAxoDebugFlags & AXODBG_BREAK_ON_ASSERT)
      {
         if (AXODBG_DebugBreak())
         {
            WPTRASSERT(pnReturnValue);
            *pnReturnValue = 0;
            return TRUE;
         }
      }
      break;
   default:
      {
         // Unexpected value - force a break
         _ShowTraceString("Unexpected value in ReportHookFn\n");
         if (AXODBG_DebugBreak())
         {
            WPTRASSERT(pnReturnValue);
            *pnReturnValue = 0;
            return TRUE;
         }
      }
   }

   // Else return FALSE so that the dialog gets shown.
   return FALSE;
}

//==============================================================================================
// FUNCTION: AXODBG_Initialize
// PURPOSE:  Setup the callback function for debug reporting.
//
void AXODBG_Initialize(void)
{
   // Setup the reporting hook function so that assertion messages 
   // get sent to the debugging context.
   static BOOL bInitialized = FALSE;
   if (!bInitialized)
   {
      _CrtSetReportHook(ReportHookFn);
      bInitialized = TRUE;
   }
}

//*********************************************************************************************
// FUNCTION: AXODBG_DebugBreak
// PURPOSE:  Protected call to DebugBreak() that only breaks if a debugger is running.
// RETURNS:  TRUE if the break occurred (i.e. the debugger was running).
//
BOOL AXODBG_DebugBreak(void)
{
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
   // If you have broken to the debugger here, you can use <Shift+F11> 
   // to jump up the call stack to the offending statement.
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
   BOOL bRval = TRUE;
   __try
   {
      _CrtDbgBreak();
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
      bRval = FALSE;
   }
   return bRval;
}

//*********************************************************************************************
// FUNCTION: AXODBG_assert
// PURPOSE:  Called by the ASSERT macro in debug builds.
//
void AXODBG_assert(LPCSTR pszExp, LPCSTR pszFile, int nLine)
{
#ifdef _DEBUG
   AXODBG_Initialize();

   int nResponse = _CrtDbgReport(_CRT_ASSERT, pszFile, nLine, NULL, "%s", pszExp);
   if (nResponse == 1)
      _CrtDbgBreak();
#endif
}

//*********************************************************************************************
// FUNCTION: AXODBG_ErrorMsg
// PURPOSE:  Called by the ERRORMSG macro in debug builds.
//
void AXODBG_ErrorMsg(LPCSTR pszFile, int nLine, LPCSTR pszFormat, ...)
{
#ifdef _DEBUG
   LPSZASSERT(pszFormat);
   AXODBG_Initialize();

   char buf[256];
   va_list args;                  
   va_start(args, pszFormat);
      
#ifdef _MSC_VER   // Use Microsoft specific length protected sprintf.
   _vsnprintf(buf, sizeof(buf)-1, pszFormat, args);
#else             // Use unprotected ANSI version.
   vsprintf(buf, pszFormat, args);
#endif

   // the _CRT_ERROR message does not have a new line at the end like the _CRT_ASSERT format.
   strcat(buf, "\n");

   int nResponse = _CrtDbgReport(_CRT_ERROR, pszFile, nLine, NULL, buf);
   if (nResponse == 1)
      _CrtDbgBreak();
#endif
}

//*********************************************************************************************
// FUNCTION: AXODBG_SystemErrorMsg
// PURPOSE:  Displays the system error in a dialog and trace output,allowing a break to the debugger.
//
void AXODBG_SystemErrorMsg(DWORD dwSystemError, LPCSTR pszFile, int nLine)
{
   TCHAR szBuf[256];
   AXODBG_GetSystemErrorText(dwSystemError, szBuf, ELEMENTS_IN(szBuf));
   AXODBG_ErrorMsg(pszFile, nLine, szBuf);
}

//*********************************************************************************************
// FUNCTION: AXODBG_SetDebugFlag
// PURPOSE:  Function to set/get debugging flags modeled after _CrtSetDbgFlag().
//
DWORD AXODBG_SetDebugFlag(DWORD dwFlags)
{
   if ((dwFlags & AXODBG_REPORT_FLAG)==0)
      s_dwAxoDebugFlags = dwFlags;
   return s_dwAxoDebugFlags;
}

//*********************************************************************************************
// FUNCTION: AXODBG_SetTracePrefix
// PURPOSE:  Function to set a prefix string used by AXODBG_printf()
//
void  AXODBG_SetTracePrefix(char const * szNewTracePrefix)
{
   // Don't hold a pointer in case someone passes a string on the stack.
   strncpy(s_szTracePrefix, szNewTracePrefix, sizeof(s_szTracePrefix)-1);
   s_szTracePrefix[sizeof(s_szTracePrefix)-1] = 0;
}

//*********************************************************************************************
// FUNCTION: AXODBG_SetTracePrefixFromModuleHandle
// PURPOSE:  Helper function to build a AXODBG_printf() prefix string from the given module handle.
//
void  AXODBG_SetTracePrefixFromModuleHandle(HMODULE mod_handle)
{
   char  app_filename[_MAX_PATH] = {0};
   char  mod_filename[_MAX_PATH] = {0};
   char  prefix[TRACE_PREFIX_LENGTH];
   
   HMODULE  exe_handle = GetModuleHandle(NULL);
   if (exe_handle)
      get_module_filename(exe_handle, app_filename);

   if(mod_handle)
      get_module_filename(mod_handle, mod_filename);

   _snprintf(prefix, sizeof(prefix), "{App=%s} {Mod=%s} ", app_filename, mod_filename);

   AXODBG_SetTracePrefix(prefix);
}

//==============================================================================================
// FUNCTION: AXODBG_printf
// PURPOSE:  A version of printf that goes to the debugging context.
//
int cdecl AXODBG_printf( char *lpsz, ... )
{
   char buf[TRACE_BUFFER_LENGTH + TRACE_PREFIX_LENGTH] = "";

   va_list args;                  
   va_start(args, lpsz);
   
   LPSZASSERT(lpsz);
   
   if (s_szTracePrefix[0])
   {
      strncpy(buf, s_szTracePrefix, sizeof(buf) - 1);
      buf[sizeof(buf) - 1] = 0;
   }

   int   length = strlen(buf);
   
   int nRval = _vsnprintf(&buf[length], sizeof(buf)-length-1, lpsz, args);
   
   _ShowTraceString( buf );
   
   return nRval;
}

//==============================================================================================
// FUNCTION:  AXODBG_GetSystemErrorText
// PURPOSE:   This function can be used to get a textual description of a WIN32 system error.
// PARAMETERS:
//    dwSystemError - the error code to produce the error message for. This is typically obtained
//                    by calling GetLastError() after a WIN32 API call has failed.
//                  - If this value is zero the string will be displayed for the value returned 
//                    by GetLastError().
// NOTES:
//    This should only be used for debugging purposes, as WIN32 error descriptions should not 
//    be used directly in Axon products except in the context of an INTERNAL ERROR message 
//    (which should not be seen in the normal course of events).
//    See also the SHOW_SYSTEM_ERROR macro that only displays the error for _DEBUG builds.
//
UINT AXODBG_GetSystemErrorText(DWORD dwSystemError, LPSTR pszBuf, UINT uMaxLen)
{
   if (dwSystemError == 0)
      dwSystemError = GetLastError();
   return FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL, dwSystemError, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
                        pszBuf, uMaxLen, NULL);
}

//==============================================================================================
// FUNCTION:  AXODBG_ShowSystemError
// PURPOSE:   This function can be used to send a textual description of a WIN32 system error out
//            to the debugging context. 
// PARAMETERS:
//    dwSystemError - the error code to produce the error message for. This is typically obtained
//                    by calling GetLastError() after a WIN32 API call has failed.
//                  - If this value is zero the string will be displayed for the value returned 
//                    by GetLastError().
// NOTES:
//    This should only be used for debugging purposes, as WIN32 error descriptions should not 
//    be used directly in Axon products except in the context of an INTERNAL ERROR message 
//    (which should not be seen in the normal course of events).
//    See also the SHOW_SYSTEM_ERROR macro that only displays the error for _DEBUG builds.
//
int AXODBG_ShowSystemError(DWORD dwSystemError)
{
   TCHAR buf[256];
   int nRval = (int)AXODBG_GetSystemErrorText(dwSystemError, buf, ELEMENTS_IN(buf));
   _ShowTraceString( buf );
   return nRval;
}


//==============================================================================================
// FUNCTION: AXODBG_IsAligned
// PURPOSE:  Returns TRUE if the given pointer is aligned on a multiple of the passed data size.
//
BOOL AXODBG_IsAligned( void *pv, UINT uDataSize )
{
   UINT uPointer = UINT(pv);
   return (uPointer % uDataSize)==0;
}
   
//==============================================================================================
// FUNCTION: AXODBG_NotOnStack
// PURPOSE:  Returns TRUE if the given pointer is not on the stack (rough check).
// NOTES:    Typically this function should be used in the constructor of large objects to check
//           that they are not being allocated on the stack.
//
BOOL AXODBG_NotOnStack( void *pv )
{
   BYTE byStackVariable = 0;
   BYTE *pby    = (BYTE *)pv;
   int   nBytes = abs(pby - &byStackVariable);

   return abs(nBytes > 4096);
}
   
