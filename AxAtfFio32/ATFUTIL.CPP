//***********************************************************************************************
//
//    Copyright (c) 1993-1995 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// FILE: ATFUTIL.CPP   A utilities module for the ATF file routines.
//
#include "wincpp.hpp"
#include "atfutil.h"

//===============================================================================================
// Variables and structures used by LoadString
//
#ifndef _WINDOWS

struct ErrorStr
{
   int   nID;
   char *szMsg;
};

static ErrorStr ErrorTable[] =
{
   #include "atferror.str"
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

#endif

