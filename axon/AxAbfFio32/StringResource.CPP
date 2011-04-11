//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// FILE: StringResource.CPP   A module containing string resource emulation code.
//
#include "wincpp.hpp"
#include "StringResource.h"

//===============================================================================================
// The LoadString function is only required if we are building the static library.
#ifdef MAK_ABFLIB_LIB

#include "Abffiles.h"

//===============================================================================================
// Variables and structures used by LoadString
//
struct ErrorStr
{
   int   nID;
   char *szMsg;
};

static ErrorStr ErrorTable[] =
{
   #include "abferror.str"
};

//===============================================================================================
// FUNCTION: LoadString
// PURPOSE:  Emulates the Windows LoadString function for non-windows environments.
//
int LoadString( HINSTANCE hInstance, int nErrorNum, char *sTxtBuf, UINT uMaxLen)
{
   UINT uErrorCount = sizeof(ErrorTable) / sizeof(ErrorStr);
   for (UINT i=0; i<uErrorCount; i++)
      if (ErrorTable[i].nID == nErrorNum)
         break;
   if (i==uErrorCount)
      return 0;
   strncpy(sTxtBuf, ErrorTable[i].szMsg, uMaxLen-1);
   sTxtBuf[uMaxLen-1] = '\0';
   return strlen(sTxtBuf);
}

#endif   // MAK_ABFLIB_LIB
