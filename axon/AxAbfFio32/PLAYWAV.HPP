//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// MODULE:  PLAYWAV.HPP
// PURPOSE: Class definition for CRecordSound class for recording .WAV files from a Sound Card.
// AUTHOR:  BHI  Jul 1995
//
#ifndef __PLAYWAV_HPP__
#define __PLAYWAV_HPP__

#include <mmsystem.h>
#pragma pack(1)

//===============================================================================================
// Structures used when writing to the file header.
//
typedef struct
{
   DWORD    lwTag; // Tag.
   DWORD    cb;    // Size of data.
} CNK;             // ChuNK.

typedef struct
{
   CNK           cnkRiff;     // 'RIFF' chunk.
   DWORD         lwWaveId;    // Must be 'WAVE'.
   CNK           cnkFormat;   // Wave specific fmt. chunk.
   WAVEFORMATEX  wft;         // PCM stuff.
   CNK           cnkData;     // PCM Data chunk.
//   BYTE          rgb[];       // The sound data.
} AFH, FAR *LPAFH;            // Audio File Header.

#pragma pack()

BOOL WriteWAVHeader(LPCSTR pszFileName, WORD wChannels, DWORD dwSamplesPerSecond, 
                    int nSampleSize, DWORD dwBytes);

#endif  // __PLAYWAV_HPP__


