/***********************************************************************************************
**
**    Copyright (c) 1993-1994 Axon Instruments.
**    All rights reserved.
**
*************************************************************************************************
** MODULE:  INITLIZE.CPP --> ABFDLL.DLL
** PURPOSE: Contains library routines for initializing the DLL.
** AUTHOR:  BHI  Feb 1994
** FUNCTIONS:
*/

#include "wincpp.hpp"
#include "axatffio32.h"

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

BOOL WINAPI DllMain( HANDLE hDLL, DWORD dwReason, LPVOID /* lpReserved */ )
{
   switch (dwReason)
   {
      case DLL_PROCESS_ATTACH:
         TRACE("AXATFFIO32: DLL_PROCESS_ATTACH.\n");
		   DisableThreadLibraryCalls((HINSTANCE)hDLL);
         ATF_Initialize(HINSTANCE(hDLL));
         break;

      case DLL_THREAD_ATTACH:
         TRACE("AXATFFIO32: DLL_THREAD_ATTACH.\n");
         break;

      case DLL_THREAD_DETACH:
         TRACE("AXATFFIO32: DLL_THREAD_DETACH.\n");
         break;

      case DLL_PROCESS_DETACH:
         TRACE("AXATFFIO32: DLL_PROCESS_DETACH.\n");
         ATF_Cleanup();
         break;
   }
   return TRUE;
}


