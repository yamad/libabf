//**********************************************************************************************
//
//    Copyright (c) 1995 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// MODULE:  RECORD.CPP
// PURPOSE: Class implementation for CRecordSound class for recording .WAV files from a Sound Card.
// AUTHOR:  BHI  Jul 1995
//
#include "wincpp.hpp"
#include "playwav.hpp"

//===============================================================================================
// FUNCTION: WriteWAVHeader
// PURPOSE:  Writes a WAV file header into the given file name.
//
BOOL WriteWAVHeader(LPCSTR pszFileName, WORD wChannels, DWORD dwSamplesPerSecond, 
                    int nSampleSize, DWORD dwBytes)
{
   AFH afh;
   memset(&afh, 0, sizeof(afh));

   HANDLE hFile = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
      return FALSE;

   // Initialize file header with constant stuff.   
   afh.cnkRiff.lwTag   = *(long *)"RIFF";   
   afh.cnkRiff.cb      = sizeof(afh) - sizeof(afh.cnkRiff);
   afh.lwWaveId        = *(long *)"WAVE";
   afh.cnkFormat.lwTag = *(long *)"fmt ";
   afh.cnkFormat.cb    = sizeof(afh.wft);
   afh.cnkData.lwTag   = *(long *)"data";
   afh.cnkData.cb      = dwBytes;
   
   afh.wft.wFormatTag      = WAVE_FORMAT_PCM;
   afh.wft.nChannels       = wChannels;
   afh.wft.nSamplesPerSec  = dwSamplesPerSecond;
   afh.wft.nAvgBytesPerSec = afh.wft.nSamplesPerSec * nSampleSize;
   afh.wft.nBlockAlign     = WORD(afh.wft.nChannels * nSampleSize);
   afh.wft.wBitsPerSample  = WORD(nSampleSize * 8);

   afh.cnkRiff.cb   += afh.cnkData.cb;

   DWORD dwWritten = 0;   
   BOOL bRval = WriteFile( hFile, &afh, sizeof(afh), &dwWritten, NULL);
   CloseHandle(hFile);
      
   return bRval;
}

