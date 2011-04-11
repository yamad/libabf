//***********************************************************************************************
//
//    Copyright (c) 1993-1999 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  AudioFileHeader.h
// PURPOSE: Contains the struct definitions Voice Tags.
// 

#ifndef INC_AUDIOFILEHEADER_H
#define INC_AUDIOFILEHEADER_H

#include <mmsystem.h>

#pragma pack(1)

//===============================================================================================
// Structures used when writing to the file header.
//
struct RiffChunk
{
   DWORD    lwTag; // Tag.
   DWORD    cb;    // Size of data.
};

struct AudioFileHeader
{
   RiffChunk     cnkRiff;     // 'RIFF' chunk.
   DWORD         dwWaveId;    // Must be 'WAVE'.
   RiffChunk     cnkFormat;   // Wave specific fmt. chunk.
   WAVEFORMATEX  WaveFormat;  // PCM stuff.
   RiffChunk     cnkData;     // PCM Data chunk.
//   BYTE          rgb[];       // The sound data.
};

#pragma pack()

#endif   // INC_AUDIOFILEHEADER_H
