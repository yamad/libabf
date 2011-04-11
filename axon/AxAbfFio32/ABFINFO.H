//***********************************************************************************************
//
//    Copyright (c) 1993-1999 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// HEADER:  ABFINFO.H
// PURPOSE: Contains definitions and prototypes for ABFINFO.CPP
//
#ifdef __cplusplus
extern "C" {
#endif

#include "abfheadr.h"
#include "AxABFFIO32.h"

#define INFO_TERSE   0
#define INFO_LONG    1
#define INFO_VERBOSE 2
#define INFO_ANNOTATIONS 3

// If this character is placed as the first character of the szDataFile field, the following
// text is used as a descriptive text for the header. Otherwise the filename component of the 
// path is stripped out and used.
#define INFO_TEXTMARKER '#'

UINT WINAPI INFO_FormatTime(long lTime, short nMillisecs, char *buf, UINT uBufLen);
UINT WINAPI INFO_FormatDate(long lDate, char *buf, UINT uBufLen);

UINT WINAPI INFO_GetBufferSize( const ABFFileHeader *pFH, LPCSTR szDataFile, int nDisplayMode);
UINT WINAPI INFO_GetInfo( const ABFFileHeader *pFH, LPCSTR szDataFile, int nDisplayMode, 
                          char *psBuffer, UINT uBufSize);

#ifdef __cplusplus
}
#endif
