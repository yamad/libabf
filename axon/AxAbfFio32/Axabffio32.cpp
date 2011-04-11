//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// MODULE:  AxABFFIO32.CPP --> AxABFFIO32.DLL
// PURPOSE: Contains library routines for initializing the DLL.
// AUTHOR:  BHI  Feb 1994
// FUNCTIONS:
//

#include "wincpp.hpp"
#include "abffiles.h"
#include "\AxonDev\Comp\Common\Version.hpp"

//----------------------------------------------------------------------
// FUNCTION:
// IsOldABFInfo
//
// PURPOSE:
// Determines if the calling application is an old version of ABFInfo
//
// RETURNS:
//    NULL     An error occurred
//    ptr      The filename
static BOOL    IsOldABFInfo()
{
   // Get the file name of the calling process.
   char     CallingCommandLine[_MAX_PATH] = {0};
   GetModuleFileName(NULL, CallingCommandLine, sizeof(CallingCommandLine)-1);
   
   // Make sure it is a long filename
   char     FullPath[_MAX_PATH] = {0};
   char     *pFileName;
   GetFullPathName(CallingCommandLine, sizeof(FullPath), FullPath, &pFileName);
   
   // Extract the file name
   // The buffer must be big enough to hold the name + the version number
   // An int is (currently) 32 bits, so the maximum length in base-10 is 32*log10(2)
   // Assume log10(2)==4 to be on the safe side.
   static char  FileName[_MAX_FNAME + 2 * (sizeof(int) * 8 * 4)] = {0};
   _splitpath(FullPath, NULL, NULL, FileName, NULL);

   if( stricmp( FileName, "ABFInfo" ) != 0 )
      return FALSE;
   
   
   // Check the version number.
   CVersion Version(FullPath);
   DWORD    dwMS = 0;
   DWORD    dwLS = 0;
   if( Version.GetFileVersion( &dwMS, &dwLS ) )
   {
      UINT uVersion = HIWORD(dwMS);
      if( uVersion < 3 )
         return TRUE;
   }
   
   return FALSE;
}


//***********************************************************************************************
//  FUNCTION:    DllMain
//  INPUTS:      hDll       - handle of DLL
//               dwReason   - indicates why DLL called
//               lpReserved - reserved
//  RETURNS:     TRUE (always, in this example.)
//               Note that the return value is used only when dwReason = DLL_PROCESS_ATTACH.
//               Normally the function would return TRUE if DLL initialization succeeded,
//               or FALSE it it failed.
//  GLOBAL VARS: g_hInstance - handle of DLL (initialized when PROCESS_ATTACHes)
//
extern "C" BOOL WINAPI DllMain( HANDLE hDLL, DWORD dwReason, LPVOID /* lpReserved */)
{
   switch (dwReason)
   {
      case DLL_PROCESS_ATTACH:
         DLLTRACE("AxABFFIO32: DLL_PROCESS_ATTACH.\n");
         if( IsOldABFInfo() )
         {
            //LoadString( IDS_CANNOTLOAD )
            MessageBox( (HWND)hDLL, "This is an old version of ABFInfo.", "Error", MB_ICONERROR || MB_OK);
            return FALSE;
         }

		   DisableThreadLibraryCalls((HINSTANCE)hDLL);
         ABF_Initialize((HINSTANCE)hDLL);
         break;

      case DLL_THREAD_ATTACH:
         DLLTRACE("AxABFFIO32: DLL_THREAD_ATTACH.\n");
         break;

      case DLL_THREAD_DETACH:
         DLLTRACE("AxABFFIO32: DLL_THREAD_DETACH.\n");
         break;

      case DLL_PROCESS_DETACH:
         DLLTRACE("AxABFFIO32: DLL_PROCESS_DETACH.\n");
         ABF_Cleanup();    
         break;
   }
   return TRUE;
}

