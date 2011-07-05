//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// HEADER:  PKWARE.H
// PURPOSE: Contains interface definition for the PKWARE wrapper module.
// AUTHOR:  BHI  Sep 95
//

#ifndef INC_PKWARE_H
#define INC_PKWARE_H

#include "..\common\FileIO.hpp"

#define SZ_PKWARE_DLL          "implode.dll"

#define CMP_BINARY             0
#define CMP_ASCII              1

#define CMP_NO_ERROR           0
#define CMP_INVALID_DICTSIZE   1
#define CMP_INVALID_MODE       2
#define CMP_BAD_DATA           3
#define CMP_ABORT              4
#define CMP_NO_MEMORY          5
#define CMP_MEMORY_ERROR       6
#define CMP_DLL_NOT_FOUND     10    // Can't load the DLL.
#define CMP_PROC_NOT_FOUND    11    // DLL does not contain the correct entry points.
#define CMP_DISKFULL          12    // Disk is full.

BOOL PKWARE_CompressFile(CFileIO &FIn, CFileIO &FOut, DWORD *pdwByteCount, DWORD *pdwCRC, int *pnError);
BOOL PKWARE_ExpandFile(CFileIO &FIn, CFileIO &FOut, DWORD *pdwCRC, int *pnError);

#endif  // INC_PKWARE_H

