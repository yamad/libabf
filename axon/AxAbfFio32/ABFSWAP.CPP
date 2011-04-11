//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// MODULE:  ABFSWAP.C
// PURPOSE: Contains functions to support byte swapping.
// AUTHOR:  BHI  May 1994
//

#include "wincpp.hpp"

#include "abfheadr.h"
#include "abfswap.h"

#define CHAR  0
#define SHORT 1
#define LONG  2
#define FLOAT 3

#pragma pack(1)
typedef struct
{
   UINT uOffset;
   BYTE byCount;
   BYTE byType;
} SWAPDEFN;

SWAPDEFN ABFHeaderSwapDef[] =
{
//   offset count   type              variable name
   {   0,    4,     CHAR     },    // lFileSignature  (Use CHAR to prevent byte swap of signature)
   {   4,    1,     FLOAT    },    // fFileVersionNumber               
   {   8,    1,     SHORT    },    // nOperationMode                   
   {   10,   1,     LONG     },    // lActualAcqLength                 
   {   14,   1,     SHORT    },    // nNumPointsIgnored                
   {   16,   1,     LONG     },    // lActualEpisodes                    
   {   20,   1,     LONG     },    // lFileStartDate                   
   {   24,   1,     LONG     },    // lFileStartTime                   
   {   28,   1,     LONG     },    // lStopwatchTime                   
   {   32,   1,     FLOAT    },    // fHeaderVersionNumber             
   {   36,   1,     SHORT    },    // nFileType                        
   {   38,   1,     SHORT    },    // nMSBinFormat                     
   {   40,   1,     LONG     },    // lDataSectionPtr                  
   {   44,   1,     LONG     },    // lTagSectionPtr                   
   {   48,   1,     LONG     },    // lNumTagEntries                   
   {   52,   1,     LONG     },    // lLongDescriptionPtr              
   {   56,   1,     LONG     },    // lLongDescriptionLines            
   {   60,   1,     LONG     },    // lDACFilePtr                      
   {   64,   1,     LONG     },    // lDACFileNumEpisodes                 
   {   68,   4,     CHAR     },    // sUnused68                        
   {   72,   1,     LONG     },    // lDeltaArrayPtr                   
   {   76,   1,     LONG     },    // lNumDeltas                       
   {   80,   1,     LONG     },    // lNotebookPtr                     
   {   84,   1,     LONG     },    // lNotebookManEntries              
   {   88,   1,     LONG     },    // lNotebookAutoEntries             
   {   92,   1,     LONG     },    // lSynchArrayPtr                   
   {   96,   1,     LONG     },    // lSynchArraySize                  
   {   100,  1,     SHORT    },    // nDataFormat
   {   102,  18,    CHAR     },    // sUnused102                       
   {   120,  1,     SHORT    },    // nADCNumChannels                  
   {   122,  1,     FLOAT    },    // fADCSampleInterval               
   {   126,  1,     FLOAT    },    // fADCSecondSampleInterval         
   {   130,  1,     FLOAT    },    // fSynchTimeUnit                   
   {   134,  1,     FLOAT    },    // fSecondsPerRun                   
   {   138,  1,     LONG     },    // lNumSamplesPerEpisode              
   {   142,  1,     LONG     },    // lPreTriggerSamples               
   {   146,  1,     LONG     },    // lEpisodesPerRun                    
   {   150,  1,     LONG     },    // lRunsPerTrial                    
   {   154,  1,     LONG     },    // lNumberOfTrials                  
   {   158,  1,     SHORT    },    // nAveragingMode                   
   {   160,  1,     SHORT    },    // nUndoRunCount                    
   {   162,  1,     SHORT    },    // nFirstEpisodeInRun               
   {   164,  1,     FLOAT    },    // fTriggerThreshold                
   {   168,  1,     SHORT    },    // nTriggerSource                   
   {   170,  1,     SHORT    },    // nTriggerAction                   
   {   172,  1,     SHORT    },    // nTriggerPolarity                 
   {   174,  1,     FLOAT    },    // fScopeOutputInterval             
   {   178,  1,     FLOAT    },    // fEpisodeStartToStart             
   {   182,  1,     FLOAT    },    // fRunStartToStart                 
   {   186,  1,     FLOAT    },    // fTrialStartToStart               
   {   190,  1,     LONG     },    // lAverageCount                    
   {   194,  1,     LONG     },    // lClockChange
   {   198,  2,     CHAR     },    // sUnused198                       
   {   200,  1,     SHORT    },    // nDrawingStrategy                 
   {   202,  1,     SHORT    },    // nTiledDisplay                    
   {   204,  1,     SHORT    },    // nEraseStrategy                   
   {   206,  1,     SHORT    },    // nDataDisplayMode                 
   {   208,  1,     LONG     },    // lDisplayAverageUpdate            
   {   212,  1,     SHORT    },    // nChannelStatsStrategy            
   {   214,  1,     LONG     },    // lCalculationPeriod               
   {   218,  1,     LONG     },    // lSamplesPerTrace                 
   {   222,  1,     LONG     },    // lStartDisplayNum                 
   {   226,  1,     LONG     },    // lFinishDisplayNum                
   {   230,  1,     SHORT    },    // nMultiColor                      
   {   232,  1,     SHORT    },    // nShowPNRawData                   
   {   234,  1,     FLOAT    },    // fStatisticsPeriod
   {   238,  6,     CHAR     },    // sUnused238                       
   {   244,  1,     FLOAT    },    // fADCRange                        
   {   248,  1,     FLOAT    },    // fDACRange                        
   {   252,  1,     LONG     },    // lADCResolution                   
   {   256,  1,     LONG     },    // lDACResolution                   
   {   260,  1,     SHORT    },    // nExperimentType                  
   {   262,  1,     SHORT    },    // nAutosampleEnable                
   {   264,  1,     SHORT    },    // nAutosampleADCNum                
   {   266,  1,     SHORT    },    // nAutosampleInstrument            
   {   268,  1,     FLOAT    },    // fAutosampleAdditGain             
   {   272,  1,     FLOAT    },    // fAutosampleFilter                
   {   276,  1,     FLOAT    },    // fAutosampleMembraneCap           
   {   280,  1,     SHORT    },    // nManualInfoStrategy              
   {   282,  1,     FLOAT    },    // fCellID1                         
   {   286,  1,     FLOAT    },    // fCellID2                         
   {   290,  1,     FLOAT    },    // fCellID3                         
   {   294,  16,    CHAR     },    // sCreatorInfo                     
   {   310,  56,    CHAR     },    // sFileComment                     
   {   366,  12,    CHAR     },    // sUnused366                       
   {   378,  16,    SHORT    },    // nADCPtoLChannelMap(0-15)         
   {   410,  16,    SHORT    },    // nADCSamplingSeq(0-15)            
   {   442,  16*10, CHAR     },    // sADCChannelName(0-15)            
   {   602,  16*8,  CHAR     },    // sADCUnits(0-15)                  
   {   730,  16,    FLOAT    },    // fADCProgrammableGain(0-15)       
   {   794,  16,    FLOAT    },    // fADCDisplayAmplification(0-15)   
   {   858,  16,    FLOAT    },    // fADCDisplayOffset(0-15)          
   {   922,  16,    FLOAT    },    // fInstrumentScaleFactor(0-15)     
   {   986,  16,    FLOAT    },    // fInstrumentOffset(0-15)          
   {   1050, 16,    FLOAT    },    // fSignalGain(0-15)                
   {   1114, 16,    FLOAT    },    // fSignalOffset(0-15)              
   {   1178, 16,    FLOAT    },    // fSignalLowpassFilter(0-15)       
   {   1242, 16,    FLOAT    },    // fSignalHighpassFilter(0-15)      
   {   1306, 4*10,  CHAR     },    // sDACChannelName(0-3)             
   {   1346, 4*8,   CHAR     },    // sDACChannelUnits(0-3)            
   {   1378, 4,     FLOAT    },    // fDACScaleFactor(0-3)             
   {   1394, 4,     FLOAT    },    // fDACHoldingLevel(0-3)            
   {   1410, 12,    CHAR     },    // nSignalType                      
   {   1412, 10,    CHAR     },    // sUnused1412                      
   {   1422, 1,     SHORT    },    // nOUTEnable                       
   {   1424, 1,     SHORT    },    // nSampleNumberOUT1                
   {   1426, 1,     SHORT    },    // nSampleNumberOUT2                
   {   1428, 1,     SHORT    },    // nFirstEpisodeOUT                 
   {   1430, 1,     SHORT    },    // nLastEpisodeOUT                  
   {   1432, 1,     SHORT    },    // nPulseSamplesOUT1                
   {   1434, 1,     SHORT    },    // nPulseSamplesOUT2                
   {   1436, 1,     SHORT    },    // nDigitalEnable                   
   {   1438, 1,     SHORT    },    // nWaveformSource                  
   {   1440, 1,     SHORT    },    // nActiveDACChannel                
   {   1442, 1,     SHORT    },    // nInterEpisodeLevel               
   {   1444, 10,    SHORT    },    // nEpochType(0-9)                  
   {   1464, 10,    FLOAT    },    // fEpochInitLevel(0-9)             
   {   1504, 10,    FLOAT    },    // fEpochLevelInc(0-9)              
   {   1544, 10,    SHORT    },    // nEpochInitDuration(0-9)          
   {   1564, 10,    SHORT    },    // nEpochDurationInc(0-9)           
   {   1584, 1,     SHORT    },    // nDigitalHolding                  
   {   1586, 1,     SHORT    },    // nDigitalInterEpisode             
   {   1588, 10,    SHORT    },    // nDigitalValue(0-9)               
   {   1612, 8,     CHAR     },    // sUnused1612                      
   {   1620, 1,     FLOAT    },    // fDACFileScale                    
   {   1624, 1,     FLOAT    },    // fDACFileOffset                   
   {   1628, 2,     CHAR     },    // sUnused1628
   {   1630, 1,     SHORT    },    // nDACFileEpisodeNum               
   {   1632, 1,     SHORT    },    // nDACFileADCNum                   
   {   1634, 12,    CHAR     },    // sDACFileName                     
   {   1646, 60,    CHAR     },    // sDACFilePath                     
   {   1706, 12,    CHAR     },    // sUnused1706                      
   {   1718, 1,     SHORT    },    // nConditEnable                    
   {   1720, 1,     SHORT    },    // nConditChannel                   
   {   1722, 1,     LONG     },    // lConditNumPulses                 
   {   1726, 1,     FLOAT    },    // fBaselineDuration                
   {   1730, 1,     FLOAT    },    // fBaselineLevel                   
   {   1734, 1,     FLOAT    },    // fStepDuration                    
   {   1738, 1,     FLOAT    },    // fStepLevel                       
   {   1742, 1,     FLOAT    },    // fPostTrainPeriod                 
   {   1746, 1,     FLOAT    },    // fPostTrainLevel                  
   {   1750, 12,    CHAR     },    // sUnused1750                      
   {   1762, 1,     SHORT    },    // nParamToVary                     
   {   1764, 80,    CHAR     },    // sParamValueList                  
   {   1844, 1,     SHORT    },    // nAutopeakEnable                  
   {   1846, 1,     SHORT    },    // nAutopeakPolarity                
   {   1848, 1,     SHORT    },    // nAutopeakADCNum                  
   {   1850, 1,     SHORT    },    // nAutopeakSearchMode              
   {   1852, 1,     LONG     },    // lAutopeakStart                   
   {   1856, 1,     LONG     },    // lAutopeakEnd                     
   {   1860, 1,     SHORT    },    // nAutopeakSmoothing               
   {   1862, 1,     SHORT    },    // nAutopeakBaseline                
   {   1864, 1,     SHORT    },    // nAutopeakAverage                 
   {   1868, 12,    CHAR     },    // sUnused1868 
   {   1880, 1,     SHORT    },    // nArithmeticEnable                
   {   1882, 1,     FLOAT    },    // fArithmeticUpperLimit            
   {   1886, 1,     FLOAT    },    // fArithmeticLowerLimit            
   {   1890, 1,     SHORT    },    // nArithmeticADCNumA               
   {   1892, 1,     SHORT    },    // nArithmeticADCNumB               
   {   1894, 1,     FLOAT    },    // fArithmeticK1                    
   {   1898, 1,     FLOAT    },    // fArithmeticK2                    
   {   1902, 1,     FLOAT    },    // fArithmeticK3                    
   {   1906, 1,     FLOAT    },    // fArithmeticK4                    
   {   1910, 2,     CHAR     },    // sArithmeticOperator              
   {   1912, 8,     CHAR     },    // sArithmeticUnits                 
   {   1920, 1,     FLOAT    },    // fArithmeticK5                    
   {   1924, 1,     FLOAT    },    // fArithmeticK6                    
   {   1928, 1,     SHORT    },    // nArithmeticExpression            
   {   1930, 2,     CHAR     },    // sUnused1930                      
   {   1932, 1,     SHORT    },    // nPNEnable                        
   {   1934, 1,     SHORT    },    // nPNPosition                      
   {   1936, 1,     SHORT    },    // nPNPolarity                      
   {   1938, 1,     SHORT    },    // nPNNumPulses                     
   {   1940, 1,     SHORT    },    // nPNADCNum                        
   {   1942, 1,     FLOAT    },    // fPNHoldingLevel                  
   {   1946, 1,     FLOAT    },    // fPNSettlingTime                  
   {   1950, 1,     FLOAT    },    // fPNInterpulse                    
   {   1954, 12,    CHAR     },    // sUnused1954                      
   {   1966, 1,     SHORT    },    // nListEnable                      
   
   {   1968, 2,     SHORT    },    // nBellEnable
   {   1972, 2,     SHORT    },    // nBellLocation
   {   1976, 2,     SHORT    },    // nBellRepetitions
   {   1980, 1,     SHORT    },    // nLevelHysteresis
   {   1982, 1,     LONG     },    // lTimeHysteresis
   {   1986, 1,     SHORT    },    // nAllowExternalTags
   
   {   1988, 16,    CHAR     },    // nLowpassFilterType
   {   2004, 16,    CHAR     },    // nHighpassFilterType
   {   2020, 2,     SHORT    },    // nVoiceTagThreshold
   {   2022, 2,     SHORT    },    // nAverageUndoPrompt
   {   2024, 24,    CHAR     },    // sUnused2024
};


SWAPDEFN ABFLogFontSwapDef[] =
{
//   offset count   type              variable name
   {   0,    1,     SHORT    },    // nHeight
   {   2,    1,     SHORT    },    // nWeight
   {   4,    4,     CHAR     },    // cPitchAndFamily and unused[3].
   {   8,   32,     CHAR     },    // szFaceName
};

SWAPDEFN ABFSignalSwapDef[] =
{
//   offset count   type              variable name
   {   0,    1,     SHORT    },    // nMxOffset
   {   4,    1,     LONG     },    // rgbColor
   {   8,    4,     CHAR     },    // nPenWidth, bDrawPoints, bHidden & bFloatData
   {   12,   1,     FLOAT    },    // fVertProportion
   {   16,   1,     FLOAT    },    // fUUTop
   {   20,   1,     FLOAT    },    // fUUBottom
};

SWAPDEFN ABFScopeConfigSwapDef[] =
{
//   offset count   type              variable name
   {   0,    1,     LONG     },    // dwFlags
   {   4,   10,     LONG     },    // rgbColor
   {   44,   1,     FLOAT    },    // fDisplayStart
   {   48,   1,     FLOAT    },    // fDisplayEnd
   {   52,   4,     CHAR     },    // bAlternateScopeMode, cUnused, bMaximized & bMinimized.
   {   56,   1,     SHORT    },    // xLeft
   {   60,   1,     SHORT    },    // yTop
   {   64,   1,     SHORT    },    // xRight
   {   68,   1,     SHORT    },    // yBottom
};

#pragma pack()

void SwapTwoBytes(void *pv, UINT uCount)
{
   BYTE lobyte, hibyte;
   BYTE *pc = (BYTE *)pv;

   while (uCount--)
   {
      lobyte = *pc;
      hibyte = *(pc+1);
      *pc++ = hibyte;
      *pc++ = lobyte;
   }
}

void SwapFourBytes(void *pv, UINT uCount)
{
   WORD loword, hiword;
   WORD *pw = (WORD *)pv;

   while (uCount--)
   {
      loword = *pw;
      hiword = *(pw+1);
      SwapTwoBytes(&loword, 1);
      SwapTwoBytes(&hiword, 1);
      *pw++ = hiword;
      *pw++ = loword;
   }
}

static void ByteSwapStructure(char *pStruct, SWAPDEFN *pSwapDefn, UINT uSwapCount)
{
   for (UINT i=0; i<uSwapCount; i++, pSwapDefn++)
   {
      switch (pSwapDefn->byType)
      {
         case CHAR:     // do nothing to CHAR items.
            break;
         case SHORT:
            SwapTwoBytes(pStruct + pSwapDefn->uOffset, pSwapDefn->byCount);
            break;
         case FLOAT:
         case LONG:
            SwapFourBytes(pStruct + pSwapDefn->uOffset, pSwapDefn->byCount);
            break;
      }
   }
}

void ByteSwapABFHeader(ABFFileHeader *pFH)
{
   ByteSwapStructure((char *)pFH, ABFHeaderSwapDef, sizeof(ABFHeaderSwapDef) / sizeof(SWAPDEFN));
}

void ByteSwapTags(ABFTag *pTagArray, UINT uCount)
{
   while (uCount--)
   {
      SwapFourBytes(&(pTagArray->lTagTime), 1);
      SwapTwoBytes(&(pTagArray->nTagType), 1);
      SwapTwoBytes(&(pTagArray->nVoiceTagNumber), 1);
      pTagArray++;
   }
}

void ByteSwapScopes(ABFScopeConfig *pCfg, UINT uCount)
{
   while (uCount--)
   {
      ByteSwapStructure((char *)pCfg, ABFScopeConfigSwapDef, 
                        sizeof(ABFScopeConfigSwapDef) / sizeof(SWAPDEFN));
      ByteSwapStructure((char *)&pCfg->LogFont, ABFLogFontSwapDef, 
                        sizeof(ABFLogFontSwapDef) / sizeof(SWAPDEFN));
      for (int i=0; i<ABF_ADCCOUNT; i++)
         ByteSwapStructure((char *)(pCfg->TraceList+i), ABFSignalSwapDef, 
                           sizeof(ABFSignalSwapDef) / sizeof(SWAPDEFN));
   }
}


#if defined TESTBED
#include <stdio.h>
#include <stdlib.h>
static short nDataBuffer[2048];

BOOL OpenFiles(char *szDataFile, HANDLE *phFileIN, HANDLE *phFileOUT)
{
   // Open input file as read only.
   HANDLE hFileIN = CreateFile(szDataFile, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFileIN == INVALID_HANDLE_VALUE)
   {
      fprintf(stderr, "Cannot open file %s.\n", szDataFile);
      return FALSE;
   }

   static char szDrive[_MAX_DRIVE], szDir[_MAX_DIR],
               szFName[_MAX_FNAME], szExt[_MAX_EXT],
               szSwapFile[_MAX_PATH];

   // Build a temporary file name.        
   _splitpath(szDataFile, szDrive, szDir, szFName, szExt);
   _makepath(szSwapFile, szDrive, szDir, szFName, "SWP");

   HANDLE hFileOUT = CreateFile(szSwapFile, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   if (hFileOUT == INVALID_HANDLE_VALUE)
   {
      CloseHandle(hFileIN);
      fprintf(stderr, "Cannot open file %s.\n", szSwapFile);
      return FALSE;
   }
   *phFileIN  = hFileIN;
   *phFileOUT = hFileOUT;
   return TRUE;
}

static BOOL ByteSwapABFFile(ABFFileHeader *pFH, HANDLE hFileIN, HANDLE hFileOUT)
{      
   // Swap the header.   
   ByteSwapABFHeader(pFH);
   SwapFourBytes(&(pFH->lFileSignature), 1);

   DWORD dwBytesWritten;   
   if (!WriteFile(hFileOUT, pFH, sizeof(ABFFileHeader), &dwBytesWritten, NULL))
   {
      printf("Error writing to output file.\n");
      return FALSE;
   }
      
   // Swap the header back again if it is a native mode file.
   if (pFH->lFileSignature != ABF_NATIVESIGNATURE)
   {
      ByteSwapABFHeader(pFH);
      SwapFourBytes(&(pFH->lFileSignature), 1);
   }
   
   if (pFH->lActualAcqLength <= 0)
   {
      printf("No data found in file.\n");
      return FALSE;
   }
   
   // Byte-swap the data section.
   SetFilePointer(hFileIN, pFH->lDataSectionPtr*ABF_BLOCKSIZE, NULL, FILE_BEGIN);
   SetFilePointer(hFileOUT, pFH->lDataSectionPtr*ABF_BLOCKSIZE, NULL, FILE_BEGIN);
   
   long lSamplesToRead = pFH->lActualAcqLength;
   long lSamplesPerBuffer = sizeof(nDataBuffer)/sizeof(short); 
   DWORD dwBytesRead;
   while (lSamplesToRead > lSamplesPerBuffer)
   {
      if (!ReadFile(hFileIN, nDataBuffer, sizeof(nDataBuffer), &dwBytesRead, NULL))
      {
         printf("Error reading input file.\n");
         return FALSE;
      }
      SwapTwoBytes(nDataBuffer, UINT(lSamplesPerBuffer));
      if (!WriteFile(hFileOUT, nDataBuffer, sizeof(nDataBuffer), &dwBytesWritten, NULL))
      {
         printf("Error writing to output file.\n");
         return FALSE;
      }
      lSamplesToRead -= lSamplesPerBuffer;
   }
   
   UINT uBytesToRead = UINT(lSamplesToRead*sizeof(short));
   if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
   {
      printf("Error reading input file.\n");
      return FALSE;
   }
   SwapTwoBytes(nDataBuffer, UINT(lSamplesToRead));
   if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
   {
      printf("Error writing to output file.\n");
      return FALSE;
   }
   
   // Byte-swap the synch array.
   if ((pFH->lSynchArrayPtr > 0) && (pFH->lSynchArraySize > 0))
   {
      SetFilePointer(hFileIN, pFH->lSynchArrayPtr*512, NULL, FILE_BEGIN);
      SetFilePointer(hFileOUT, pFH->lSynchArrayPtr*512, NULL, FILE_BEGIN);
      
      long lSynchEntries = pFH->lSynchArraySize;
      long lSynchsPerBuffer = sizeof(nDataBuffer)/sizeof(ABFSynch);
      UINT uBytesToRead = UINT(lSynchsPerBuffer * sizeof(ABFSynch));
      while (lSynchEntries > lSynchsPerBuffer)
      {
         if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
         {
            printf("Error reading input file.\n");
            return FALSE;
         }
         SwapFourBytes(nDataBuffer, UINT(lSynchsPerBuffer * sizeof(ABFSynch)/sizeof(long)));
         if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
         {
            printf("Error writing to output file.\n");
            return FALSE;
         }
         lSynchEntries -= lSynchsPerBuffer;
      }
      
      uBytesToRead = UINT(lSynchEntries*sizeof(ABFSynch));
      if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
      {
         printf("Error reading input file.\n");
         return FALSE;
      }
      SwapFourBytes(nDataBuffer, UINT(lSynchEntries * sizeof(ABFSynch)/sizeof(long)));
      if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
      {
         printf("Error writing to output file.\n");
         return FALSE;
      }
   }
   
   // Byte-swap the tag array.
   if ((pFH->lTagSectionPtr > 0) && (pFH->lNumTagEntries > 0))
   {
      SetFilePointer(hFileIN, pFH->lTagSectionPtr*512, NULL, FILE_BEGIN);
      SetFilePointer(hFileOUT, pFH->lTagSectionPtr*512, NULL, FILE_BEGIN);
      
      long lTagEntries = pFH->lNumTagEntries;
      long lTagsPerBuffer = sizeof(nDataBuffer)/sizeof(ABFTag);
      UINT uBytesToRead = UINT(lTagsPerBuffer * sizeof(ABFTag));
      while (lTagEntries > lTagsPerBuffer)
      {
         if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
         {
            printf("Error reading input file.\n");
            return FALSE;
         }
         ByteSwapTags((ABFTag *)nDataBuffer, UINT(lTagsPerBuffer));
         if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
         {
            printf("Error writing to output file.\n");
            return FALSE;
         }
         lTagEntries -= lTagsPerBuffer;
      }
      
      uBytesToRead = UINT(lTagEntries*sizeof(ABFTag));
      if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
      {
         printf("Error reading input file.\n");
         return FALSE;
      }
      ByteSwapTags((ABFTag *)nDataBuffer, UINT(lTagEntries));
      if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
      {
         printf("Error writing to output file.\n");
         return FALSE;
      }
   }
   
   // Byte swap the DAC file section.
   if ((pFH->lDACFilePtr > 0) && (pFH->lDACFileNumEpisodes > 0))
   {
      SetFilePointer(hFileIN, pFH->lDACFilePtr*512, NULL, FILE_BEGIN);
      SetFilePointer(hFileOUT, pFH->lDACFilePtr*512, NULL, FILE_BEGIN);
      
      long lDACFileSamples = pFH->lDACFileNumEpisodes * pFH->lNumSamplesPerEpisode;
      UINT uBytesToRead = UINT(lSamplesPerBuffer * sizeof(short));
      while (lDACFileSamples > lSamplesPerBuffer)
      {
         if (!ReadFile(hFileIN, nDataBuffer, sizeof(nDataBuffer), &dwBytesRead, NULL))
         {
            printf("Error reading input file.\n");
            return FALSE;
         }
         SwapTwoBytes(nDataBuffer, UINT(lSamplesPerBuffer));
         if (!WriteFile(hFileOUT, nDataBuffer, sizeof(nDataBuffer), &dwBytesWritten, NULL))
         {
            printf("Error writing to output file.\n");
            return FALSE;
         }
         lDACFileSamples -= lSamplesPerBuffer;
      }
      
      uBytesToRead = UINT(lDACFileSamples*sizeof(short));
      if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
      {
         printf("Error reading input file.\n");
         return FALSE;
      }
      SwapTwoBytes(nDataBuffer, UINT(lDACFileSamples));
      if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
      {
         printf("Error writing to output file.\n");
         return FALSE;
      }
   }
   
   return TRUE;
}

static BOOL ByteSwapOldPclampFile(float *pF, HANDLE hFileIN, HANDLE hFileOUT)
{
   BOOL bSwapped = FALSE;
   if ((pF[0]!=1.0F) && (pF[0]!=10.0F))
   {
      SwapFourBytes(pF, 80);
      bSwapped = TRUE;
      if ((pF[0]!=1.0F) && (pF[0]!=10.0F))
      {   
         printf("Unknown file type.\n");
         return FALSE;
      }
   }
   if (pF[8] < 5.19999F)
   {
      printf("ABFSwap does not support pCLAMP file prior to V5.2.\n");
      return FALSE;
   }
   if (!bSwapped)
      SwapFourBytes(pF, 80);

   UINT uBytesToWrite = 80 * sizeof(float);
   DWORD dwBytesWritten;
   if (!WriteFile(hFileOUT, pF, uBytesToWrite, &dwBytesWritten, NULL))
   {
      printf("Error writing to file.\n");
      return FALSE;
   }
   
   if (!bSwapped)
      SwapFourBytes(pF, 80);

   UINT uBytesToRead = 77+80+35+64;      
   DWORD dwBytesRead;
   if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
   {
      printf("Error reading file.\n");
      return FALSE;
   }
   if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
   {
      printf("Error writing file.\n");
      return FALSE;
   }
   
   uBytesToRead = 64+384-16*8;
   if (!ReadFile(hFileIN, nDataBuffer, uBytesToRead, &dwBytesRead, NULL))
   {
      printf("Error reading file.\n");
      return FALSE;
   }
   SwapFourBytes(nDataBuffer, uBytesToRead / sizeof(float));
   if (!WriteFile(hFileOUT, nDataBuffer, uBytesToRead, &dwBytesWritten, NULL))
   {
      printf("Error writing file.\n");
      return FALSE;
   }
   SetFilePointer(hFileIN, 1024, NULL, FILE_BEGIN);
   SetFilePointer(hFileOUT, 1024, NULL, FILE_BEGIN);

   DWORD dwBytesRead;   
   while (1)
   {
      ReadFile(hFileIN, nDataBuffer, sizeof(nDataBuffer), &dwBytesRead, NULL);
      if (dwBytesRead > 0)
      {
         SwapTwoBytes(nDataBuffer, UINT(dwBytesRead) / sizeof(short));
         if (!WriteFile(hFileOUT, nDataBuffer, dwBytesRead, &dwBytesWritten, NULL))
         {
            printf("Error writing file.\n");
            return FALSE;
         }
      }      
      if (dwBytesRead!=sizeof(nDataBuffer))
         return TRUE;
   }   
}

static BOOL ByteSwapFile(HANDLE hFileIN, HANDLE hFileOUT)
{
   BOOL bRval = FALSE;
   ABFFileHeader FH;
   
   DWORD dwBytesRead;
   if (!ReadFile(hFileIN, &FH, sizeof(FH), &dwBytesRead, NULL))
   {
      printf("Error reading input file.\n");
      return FALSE;
   }
      
   if ((FH.lFileSignature == ABF_NATIVESIGNATURE) ||
       (FH.lFileSignature == ABF_REVERSESIGNATURE))
      return ByteSwapABFFile(&FH, hFileIN, hFileOUT);
   
   float pF[80];
     
   SetFilePointer(hFileIN, 0, NULL, FILE_BEGIN);
   if (!ReadFile(hFileIN, pF, sizeof(pF), &dwBytesRead, NULL))
   {
      printf("Error reading file.\n");
      return FALSE;
   }
   
   return ByteSwapOldPclampFile(pF, hFileIN, hFileOUT);
}

void cdecl main(int argc, char **argv)
{
   HANDLE hFileIN, hFileOUT;

   --argc;
   ++argv;
   
   printf("ABFSWAP V1.0.0.00 -- Byte-swaps ABF files.\n");

   for (;argc--; argv++)
   {
      printf("File: %s\n", *argv);
      if (!OpenFiles(*argv, &hFileIN, &hFileOUT))
         continue;
      
      if (!ByteSwapFile(hFileIN, hFileOUT))
         fprintf(stderr, "Bad file: %s.\n", *argv);
         
      CloseHandle(hFileIN);
      CloseHandle(hFileOUT);
   }
}

#endif

