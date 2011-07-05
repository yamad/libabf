//***********************************************************************************************
//
//    Copyright (c) 1993-2002 Axon Instruments, Inc.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  ABFVTAGS.CPP
// PURPOSE: Contains the functions for adding and accessing Voice Tags from an ABF file.
// 
// An ANSI C++ compiler should be used for compilation.
// Compile with the large memory model option.
// (e.g. CL -c -AL ABFFILES.C)

#include "wincpp.hpp"
#include "abffiles.h"
#include "abfutil.h"                // Large memory allocation/free
#include "..\Common\WaitCursor.hpp"
#include "AudioFileHeader.h"
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"     // for AXU_* functions

//===============================================================================================
// FUNCTION: WriteWAVHeader
// PURPOSE:  Writes a WAV file header into the given file name.
//
static BOOL WriteWAVHeader(LPCSTR pszFileName, ABFVoiceTagInfo *pVTI)
{
   HANDLE hFile = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
      return FALSE;

   WORD  wChannels          = min(pVTI->wChannels, WORD(1));
   DWORD dwSamplesPerSecond = pVTI->lSamplesPerSecond;
   int nSampleSize          = pVTI->nSampleSize;
   DWORD dwBytes            = pVTI->lUncompressedSize;

   AudioFileHeader afh;
   memset(&afh, 0, sizeof(afh));

   // Initialize file header with constant stuff.   
   afh.cnkRiff.lwTag   = *(long *)"RIFF";   
   afh.cnkRiff.cb      = sizeof(afh) - sizeof(afh.cnkRiff) + dwBytes;
   afh.dwWaveId        = *(long *)"WAVE";
   afh.cnkFormat.lwTag = *(long *)"fmt ";
   afh.cnkFormat.cb    = sizeof(afh.WaveFormat);
   afh.cnkData.lwTag   = *(long *)"data";
   afh.cnkData.cb      = dwBytes;
   
   afh.WaveFormat.wFormatTag      = WAVE_FORMAT_PCM;
   afh.WaveFormat.nChannels       = wChannels;
   afh.WaveFormat.nSamplesPerSec  = dwSamplesPerSecond;
   afh.WaveFormat.nAvgBytesPerSec = dwSamplesPerSecond * nSampleSize;
   afh.WaveFormat.nBlockAlign     = WORD(wChannels * nSampleSize);
   afh.WaveFormat.wBitsPerSample  = WORD(nSampleSize * 8);

   DWORD dwWritten = 0;   
   BOOL bRval = WriteFile( hFile, &afh, sizeof(afh), &dwWritten, NULL);
   CloseHandle(hFile);
      
   return bRval;
}

//===============================================================================================
// FUNCTION: PlaySoundThreadProc
// PURPOSE:  Static thread function to play a wave file and delete it after it has finished.
//
static DWORD WINAPI PlaySoundThreadProc(void *pvData)
{
   if (!pvData)
      return 0;

   // The filename of the file to play is passed in as strdup'ed memory.
   LPSTR pszWAVFile = (LPSTR)pvData;
   LPSZASSERT(pszWAVFile);

   // If the user has a sound scheme setup, the sound card could be in use playing
   // the "click" that corresponds to a menu selection, so we will wait out a reasonable
   // length of time (one second) before giving up.
   UINT uMaxTries = 10;
   while ( uMaxTries-- && !PlaySound(pszWAVFile, NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT) )
   {
      TRACE("PlaySound failed.\n");
      Sleep(100);
   }

   // Even if the sound is played synchronously, the file doesn't become unlocked immediately.
   // Wait a max of one second and then give up if it still cannot be deleted.
   uMaxTries = 10;
   while (uMaxTries-- && !DeleteFile(pszWAVFile) )
   {
      TRACE("Delete failed.\n");
      SHOW_SYSTEM_ERROR(GetLastError());
      Sleep(100);
   }

   // Don't forget to free the strdup'ed parameter.
   free(pszWAVFile);

   return 0;
}

//===============================================================================================
// FUNCTION: ABF_PlayVoiceTag
// PURPOSE:  Retrieves a voice tag, builds a WAV file, plays the WAV file and cleans up.
//
BOOL WINAPI ABF_PlayVoiceTag( int nFile, const ABFFileHeader *pFH, UINT uTag, int *pnError)
{
   // Set the hour-glass cursor.
   CWaitCursor WaitCursor;

   // Create a unique temporary filename
   char szWAVFile[_MAX_PATH];
   AXU_GetTempFileName("wav", 0, szWAVFile);

   // Extract the voice tag to the temp file.
   ABFVoiceTagInfo VTI;
   if (!ABF_GetVoiceTag(nFile, pFH, uTag, szWAVFile, 
                        sizeof(AudioFileHeader), &VTI, pnError))
      return FALSE;

   // Set-up the header of the WAV file.
   if (!WriteWAVHeader(szWAVFile, &VTI))
   {
      if (pnError)
         *pnError = ABF_BADTEMPFILE;
      return FALSE;
   }

   // Spin off a thread to play the tag so that it can be deleted when it finishes.
   DWORD dwThreadId = 0;
   HANDLE h = CreateThread(NULL,                // thread security attributes  
                           1024,                // thread stack size (bytes)
                           PlaySoundThreadProc, // thread function 
                           strdup(szWAVFile),   // thread argument
                           0,                   // creation flags 
                           &dwThreadId);        // returned thread identifier 

   // Close the thread handle so that it gets cleaned up when it terminates.
   CloseHandle(h);
   return TRUE;
}

