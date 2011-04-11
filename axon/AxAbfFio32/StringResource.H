//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// HEADER:  StringResource.H    Prototypes for functions in StringResource.CPP
// AUTHOR:  BHI  Feb 1995

#ifndef INC_StringResource_H
#define INC_StringResource_H

extern HINSTANCE g_hInstance;
#include "resource.h"

//===============================================================================================
// The LoadString function is only required if we are building the static library.
#ifdef MAK_ABFLIB_LIB
int LoadString( HINSTANCE hInstance, int nErrorNum, char *sTxtBuf, UINT uMaxLen);
#endif   // MAK_ABFLIB_LIB

#endif   // INC_StringResource_H
