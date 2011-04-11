//***********************************************************************************************
//
//    Copyright (c) 2005 Molecular Devices.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  ProtocolWriterABF2.CPP
// PURPOSE: Writes ABF 2 protocols to an ABF 2 file.
// 

#include "wincpp.hpp"
#include "ProtocolWriterABF2.hpp"
#include "abfutil.h"                // Large memory allocation/free
#include "filedesc.hpp"             // File descriptors for ABF files.
#include "\AxonDev\Comp\AxoUtils32\AxoUtils32.h"     // for AXU_* functions
#include "\AxonDev\Comp\Common\ArrayPtr.hpp"

//===============================================================================================
// Send out struct size details to AxoTrace.
void ShowStructSizes()
{
   AXODBG_SHOW_SIZE(ABFFileHeader);
   AXODBG_SHOW_SIZE(ABF_FileInfo);
   AXODBG_SHOW_SIZE(ABF_ProtocolInfo);
   AXODBG_SHOW_SIZE(ABF_ADCInfo);
   AXODBG_SHOW_SIZE(ABF_DACInfo);
   AXODBG_SHOW_SIZE(ABF_EpochInfoPerDAC);
   AXODBG_SHOW_SIZE(ABF_EpochInfo);
   AXODBG_SHOW_SIZE(ABF_StatsRegionInfo);
   AXODBG_SHOW_SIZE(ABF_UserListInfo);
   AXODBG_SHOW_SIZE(ABF_MathInfo);
}

//===============================================================================================
// CProtocolWriterABF2 implementation
//===============================================================================================
// NOTE:   Any changes made here will require complementary changes to ProtocolReader.cpp

//===============================================================================================
// Constructor.
//
CProtocolWriterABF2::CProtocolWriterABF2( CFileDescriptor *pFI )
: CProtocolWriter( pFI )
{
   MEMBERASSERT();

   m_pFH        = NULL;
   m_lNextBlock = 0;

#if _DEBUG
   ShowStructSizes();
#endif
}


//===============================================================================================
// Destructor.
//
CProtocolWriterABF2::~CProtocolWriterABF2()
{
   MEMBERASSERT();

}


//===============================================================================================
// FUNCTION: CanSave
// PURPOSE:  Returns TRUE if this write can save the file
//
BOOL CProtocolWriterABF2::CanSave( const ABFFileHeader *pFH )
{
   ASSERT( pFH );

   // If the file version is less than 2, then we will save it.
   if (pFH->fFileVersionNumber >= ABF_V200)
      return TRUE;

   return FALSE;
}

//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Writes the complete protocol to the data file.
//
BOOL CProtocolWriterABF2::Write( const ABF_FileInfo *pOldFileInfo, ABFFileHeader *pFH )
{
   MEMBERASSERT();
   RPTRASSERT( pFH );

   m_pFH = pFH;

   BOOL bOK = TRUE;

   // Clear the existing file contents if no data has been written.
   if( m_pFI->GetAcquiredSamples() == 0 )
      bOK &= m_pFI->SetEndOfFile();

   if( pOldFileInfo )
      m_FileInfo.StringsSection = pOldFileInfo->StringsSection;

   // Clear the strings since we are about to re-write them.
   m_Strings.Clear();

   // Write out the various bits.
   bOK &= WriteFileInfo();
   bOK &= WriteProtocolInfo();
   bOK &= WriteADCInfo();
   bOK &= WriteDACInfo();
   bOK &= WriteEpochs();
   bOK &= WriteStats();
   bOK &= WriteUserList();
   bOK &= WriteMathInfo();

   // Write out the protocol strings.
   bOK &= WriteStrings();

   // Now re-write the FileInfo to update the the section info.
   bOK &= m_pFI->Seek( 0L, FILE_BEGIN);
   bOK &= m_pFI->Write( &m_FileInfo, sizeof( m_FileInfo ) );
   bOK &= m_pFI->Seek( 0L, FILE_END);
   m_pFI->FillToNextBlock( &m_lNextBlock );

   if( m_pFI->GetAcquiredSamples() == 0 )
   {
      m_FileInfo.DataSection.uBlockIndex = m_lNextBlock;
      m_pFH->lDataSectionPtr             = m_lNextBlock;
   }

   return bOK;
}

static UINT BytesToBlocks( UINT uBytes )
{
   UINT uBlocks = uBytes / ABF_BLOCKSIZE;
   if( uBytes % ABF_BLOCKSIZE )
      uBlocks++;

   return uBlocks;
}

//===============================================================================================
// FUNCTION: WriteStrings
// PURPOSE:  Write the Protocol Strings from the cache to the file.
//
BOOL CProtocolWriterABF2::WriteStrings()
{
   MEMBERASSERT();
   
   long lCount = m_Strings.GetNumStrings();
   if( lCount == 0 )
   {
      m_FileInfo.StringsSection.Set( 0, 0, 0 );
      return TRUE;
   }

   // Pad out the current block.
   m_pFI->FillCurrentBlock( &m_lNextBlock );

   // Work out if we can overwrite the existing string table or need to create a new one.
   if( m_FileInfo.StringsSection.uBytes )
   {
      UINT uOldNumBlocks = BytesToBlocks( m_FileInfo.StringsSection.uBytes );
      UINT uNewNumBlocks = BytesToBlocks( m_Strings.GetTotalSize() );
      if( uNewNumBlocks <= uOldNumBlocks )
      {
         // Strings fit into the original space - overwrite the old table with the new one.
         m_pFI->Seek( m_FileInfo.StringsSection.uBlockIndex * ABF_BLOCKSIZE, FILE_BEGIN );
      }
      else
      {
         // Strings need more blocks - rewrite them at the end (and orphan the original table).
         m_pFI->FillToNextBlock( &m_lNextBlock );
      }
   }
   
   UINT uOffset = 0;
   if( !m_Strings.Write( m_pFI->GetFileHandle(), uOffset ) )
      return FALSE;

   // Update the FileInfo.
   m_FileInfo.StringsSection.Set( m_lNextBlock, m_Strings.GetTotalSize(), lCount );
   m_pFI->FillCurrentBlock( &m_lNextBlock );

   // Clear the cache.
   m_Strings.Clear();
   
   return TRUE;
}

#define ADD_STRING( s )  AddString((s), ELEMENTS_IN(s))

//===============================================================================================
// FUNCTION: AddString
// PURPOSE:  Adds a new protocol string to the cache.
// RETURNS:  The index in the cache of this string (NOT the total number of strings!).
//
UINT CProtocolWriterABF2::AddString( LPCSTR pszText, UINT uLen )
{
   MEMBERASSERT();
   ASSERT( uLen <= ABF_PATHLEN );
   ASSERT( uLen != 4 );              // sizeof( int) - in case a pointer is wrongly passed in.

   char sz[ABF_PATHLEN];
   AXU_strncpyz( sz, pszText, uLen ); 
   AXU_StripWhiteSpace( sz );

   if( strlen( sz ) == 0 )
      return 0;
   
   return m_Strings.Add( sz );
}

//===============================================================================================
// FUNCTION: GetVersion
// PURPOSE:  Builds a Version number as a packed UINT from 4 shorts.
//
static UINT GetVersion( short nMajor, short nMinor, short nBugFix, short nBuild )
{
   WORD wLow     = MAKEWORD( nBuild, nBugFix );
   WORD wHigh    = MAKEWORD( nMinor, nMajor );
   UINT uVersion = MAKELONG( wLow, wHigh );

   return uVersion;
}

//===============================================================================================
// FUNCTION: GetVersion
// PURPOSE:  Builds a Version number as a packed UINT from a float (used by ABF 1.x version numbers).
//
static UINT GetVersion( float fVersion )
{
   char szVersion[16] = { 0 };
   char szMajor[16]   = { 0 };
   char szMinor[16]   = { 0 };
   AXU_sprintf( szVersion, ELEMENTS_IN( szVersion ), "%1.2f", fVersion );     // 1.83
   AXU_strncpyz( szMajor, &szVersion[0], 2 );                                  // "1"
   AXU_strncpyz( szMinor, &szVersion[2], 3 );                                  // "83"


   UINT uMajor = atoi( szMajor );
   UINT uMinor = atoi( szMinor );
   WORD wHigh    = MAKEWORD( uMinor, uMajor );
   UINT uVersion = MAKELONG( 0, wHigh );

   return uVersion;
}

//===============================================================================================
// FUNCTION: WriteFileInfo
// PURPOSE:  Writes the file info to the data file.
//
BOOL CProtocolWriterABF2::WriteFileInfo()
{
   MEMBERASSERT();

   BOOL bOK = TRUE;

   UINT uAcquiredSamples = m_pFI->GetAcquiredSamples();
   if( uAcquiredSamples )
   {
      // Make sure that at least one of the date fields has been set.
      ASSERT( m_pFH->uFileStartDate || m_pFH->uFileStartTimeMS );
   }

   m_FileInfo.uFileVersionNumber     = GetVersion( ABF_CURRENTVERSION );
                                       
   m_FileInfo.nFileType              = m_pFH->nFileType;
   m_FileInfo.nDataFormat            = m_pFH->nDataFormat;
   m_FileInfo.nSimultaneousScan      = m_pFH->nSimultaneousScan;
   m_FileInfo.FileGUID               = m_pFH->FileGUID;
   m_FileInfo.uFileCRC               = m_pFH->ulFileCRC;
   m_FileInfo.nCRCEnable             = m_pFH->nCRCEnable;

   m_FileInfo.uCreatorVersion        = GetVersion( m_pFH->nCreatorMajorVersion, 
                                                   m_pFH->nCreatorMinorVersion, 
                                                   m_pFH->nCreatorBugfixVersion, 
                                                   m_pFH->nCreatorBuildVersion );
   m_FileInfo.uModifierVersion       = GetVersion( m_pFH->nModifierMajorVersion, 
                                                   m_pFH->nModifierMinorVersion, 
                                                   m_pFH->nModifierBugfixVersion, 
                                                   m_pFH->nModifierBuildVersion );

   m_FileInfo.uFileStartDate         = m_pFH->uFileStartDate;
   m_FileInfo.uFileStartTimeMS       = m_pFH->uFileStartTimeMS;
   m_FileInfo.uStopwatchTime         = m_pFH->lStopwatchTime;

   m_FileInfo.uActualEpisodes        = m_pFI->GetAcquiredEpisodes();

   m_FileInfo.uCreatorNameIndex  = ADD_STRING( m_pFH->sCreatorInfo );
   m_FileInfo.uModifierNameIndex = ADD_STRING( m_pFH->sModifierInfo );
   m_FileInfo.uProtocolPathIndex = ADD_STRING( m_pFH->sProtocolPath );

   // Update the section pointers from the protocol.
   m_FileInfo.ScopeSection.Set( m_pFH->lScopeConfigPtr, sizeof( ABFScopeConfig ), m_pFH->lNumScopes );
   m_FileInfo.StatsSection.Set( m_pFH->lStatisticsConfigPtr, sizeof( ABFScopeConfig ), 1 );

   UINT uSampleSize = (m_pFH->nDataFormat != ABF_INTEGERDATA) ? sizeof(float) : sizeof(short);
   m_FileInfo.DataSection.Set( m_pFH->lDataSectionPtr, uSampleSize, uAcquiredSamples );
   m_FileInfo.TagSection.Set( m_pFH->lTagSectionPtr, sizeof( ABFTag ), m_pFH->lNumTagEntries );
   m_FileInfo.DeltaSection.Set( m_pFH->lDeltaArrayPtr, sizeof( ABFDelta ), m_pFH->lNumDeltas );
   m_FileInfo.VoiceTagSection.Set( m_pFH->lVoiceTagPtr, 0, m_pFH->lVoiceTagEntries );
   m_FileInfo.SynchArraySection.Set( m_pFH->lSynchArrayPtr, sizeof( ABFSynch ), m_pFH->lSynchArraySize );
   m_FileInfo.AnnotationSection.Set( m_pFH->lAnnotationSectionPtr, 0, m_pFH->lNumAnnotations );

   bOK &= m_pFI->Seek( 0L, FILE_BEGIN);
   bOK &= m_pFI->Write( &m_FileInfo, sizeof( m_FileInfo ) );
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return TRUE;
}

//===============================================================================================
// FUNCTION: WriteProtocolInfo
// PURPOSE:  Writes the protocol info to the data file.
//
BOOL CProtocolWriterABF2::WriteProtocolInfo()
{
   MEMBERASSERT();

   BOOL bOK = TRUE;   
   
   ABF_ProtocolInfo Protocol;
   Protocol.nOperationMode               = m_pFH->nOperationMode;
   Protocol.fADCSequenceInterval         = m_pFH->fADCSequenceInterval;
   Protocol.bEnableFileCompression       = m_pFH->bEnableFileCompression;
   Protocol.uFileCompressionRatio        = m_pFH->uFileCompressionRatio;

   Protocol.fSynchTimeUnit               = m_pFH->fSynchTimeUnit;
   Protocol.fSecondsPerRun               = m_pFH->fSecondsPerRun;
   Protocol.lNumSamplesPerEpisode        = m_pFH->lNumSamplesPerEpisode;
   Protocol.lPreTriggerSamples           = m_pFH->lPreTriggerSamples;
   Protocol.lEpisodesPerRun              = m_pFH->lEpisodesPerRun;
   Protocol.lRunsPerTrial                = m_pFH->lRunsPerTrial;
   Protocol.lNumberOfTrials              = m_pFH->lNumberOfTrials;
   Protocol.nAveragingMode               = m_pFH->nAveragingMode;
   Protocol.nUndoRunCount                = m_pFH->nUndoRunCount;
   Protocol.nFirstEpisodeInRun           = m_pFH->nFirstEpisodeInRun;
   Protocol.fTriggerThreshold            = m_pFH->fTriggerThreshold;
   Protocol.nTriggerSource               = m_pFH->nTriggerSource;
   Protocol.nTriggerAction               = m_pFH->nTriggerAction;
   Protocol.nTriggerPolarity             = m_pFH->nTriggerPolarity;
   Protocol.fScopeOutputInterval         = m_pFH->fScopeOutputInterval;
   Protocol.fEpisodeStartToStart         = m_pFH->fEpisodeStartToStart;
   Protocol.fRunStartToStart             = m_pFH->fRunStartToStart;
   Protocol.lAverageCount                = m_pFH->lAverageCount;
   Protocol.fTrialStartToStart           = m_pFH->fTrialStartToStart;
   Protocol.nAutoTriggerStrategy         = m_pFH->nAutoTriggerStrategy;
   Protocol.fFirstRunDelayS              = m_pFH->fFirstRunDelayS;
   Protocol.nChannelStatsStrategy        = m_pFH->nChannelStatsStrategy;
   Protocol.lSamplesPerTrace             = m_pFH->lSamplesPerTrace;
   Protocol.lStartDisplayNum             = m_pFH->lStartDisplayNum;
   Protocol.lFinishDisplayNum            = m_pFH->lFinishDisplayNum;
   Protocol.nShowPNRawData               = m_pFH->nShowPNRawData;
   Protocol.fStatisticsPeriod            = m_pFH->fStatisticsPeriod;
   Protocol.lStatisticsMeasurements      = m_pFH->lStatisticsMeasurements;
   Protocol.nStatisticsSaveStrategy      = m_pFH->nStatisticsSaveStrategy;
   Protocol.fADCRange                    = m_pFH->fADCRange;
   Protocol.fDACRange                    = m_pFH->fDACRange;
   Protocol.lADCResolution               = m_pFH->lADCResolution;
   Protocol.lDACResolution               = m_pFH->lDACResolution;
   Protocol.nDigitizerADCs               = m_pFH->nDigitizerADCs;
   Protocol.nDigitizerDACs               = m_pFH->nDigitizerDACs;
   Protocol.nDigitizerTotalDigitalOuts   = m_pFH->nDigitizerTotalDigitalOuts;
   Protocol.nDigitizerSynchDigitalOuts   = m_pFH->nDigitizerSynchDigitalOuts;
   Protocol.nDigitizerType               = m_pFH->nDigitizerType;

   Protocol.nExperimentType              = m_pFH->nExperimentType;
   Protocol.nManualInfoStrategy          = m_pFH->nManualInfoStrategy;
   Protocol.fCellID[0]                   = m_pFH->fCellID1;
   Protocol.fCellID[1]                   = m_pFH->fCellID2;
   Protocol.fCellID[2]                   = m_pFH->fCellID3;
   Protocol.nCommentsEnable              = m_pFH->nCommentsEnable;
   Protocol.nAutoAnalyseEnable           = m_pFH->nAutoAnalyseEnable;
   Protocol.nSignalType                  = m_pFH->nSignalType;
   Protocol.nDigitalEnable               = m_pFH->nDigitalEnable;
   Protocol.nActiveDACChannel            = m_pFH->nActiveDACChannel;
   Protocol.nDigitalHolding              = m_pFH->nDigitalHolding;
   Protocol.nDigitalInterEpisode         = m_pFH->nDigitalInterEpisode;
   Protocol.nDigitalDACChannel           = m_pFH->nDigitalDACChannel;
   Protocol.nDigitalTrainActiveLogic     = m_pFH->nDigitalTrainActiveLogic;
   Protocol.nStatsEnable                 = m_pFH->nStatsEnable;
   Protocol.nLevelHysteresis             = m_pFH->nLevelHysteresis;
   Protocol.lTimeHysteresis              = m_pFH->lTimeHysteresis;
   Protocol.nAllowExternalTags           = m_pFH->nAllowExternalTags;
   Protocol.nAverageAlgorithm            = m_pFH->nAverageAlgorithm;
   Protocol.fAverageWeighting            = m_pFH->fAverageWeighting;
   Protocol.nUndoPromptStrategy          = m_pFH->nUndoPromptStrategy;
   Protocol.nTrialTriggerSource          = m_pFH->nTrialTriggerSource;
   Protocol.nStatisticsDisplayStrategy   = m_pFH->nStatisticsDisplayStrategy;
   Protocol.nExternalTagType             = m_pFH->nExternalTagType;
   Protocol.nStatisticsClearStrategy     = m_pFH->nStatisticsClearStrategy;
   Protocol.nLTPType                     = m_pFH->nLTPType;
   Protocol.nScopeTriggerOut             = m_pFH->nScopeTriggerOut;
   Protocol.nAlternateDACOutputState     = m_pFH->nAlternateDACOutputState;
   Protocol.nAlternateDigitalOutputState = m_pFH->nAlternateDigitalOutputState;

   Protocol.lFileCommentIndex            = ADD_STRING( m_pFH->sFileComment );

   m_FileInfo.ProtocolSection.Set( m_lNextBlock, sizeof( Protocol ), 1 );
   bOK &= m_pFI->Seek( m_FileInfo.ProtocolSection.uBlockIndex * ABF_BLOCKSIZE, FILE_BEGIN);
   bOK &= m_pFI->Write( &Protocol, sizeof( Protocol ) );
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return bOK;
}

//===============================================================================================
// FUNCTION: WriteADCInfo
// PURPOSE:  Writes the ADC info to the data file.
//
BOOL CProtocolWriterABF2::WriteADCInfo()
{
   MEMBERASSERT();

   BOOL bOK = TRUE;
   ABF_ADCInfo ADCInfo;

   // Channel Index. The channel index is stored in the Sampling Sequence.
   UINT ch = 0;

   for( int a=0; a<m_pFH->nADCNumChannels; a++ )
   {
      // Get the channel index from the sampling sequence array.
      ch = m_pFH->nADCSamplingSeq[a];

      ADCInfo.nADCNum                       = short(ch); 
      ADCInfo.nTelegraphEnable              = m_pFH->nTelegraphEnable[ch]; 
      ADCInfo.nTelegraphInstrument          = m_pFH->nTelegraphInstrument[ch]; 
      ADCInfo.fTelegraphAdditGain           = m_pFH->fTelegraphAdditGain[ch]; 
      ADCInfo.fTelegraphFilter              = m_pFH->fTelegraphFilter[ch]; 
      ADCInfo.fTelegraphMembraneCap         = m_pFH->fTelegraphMembraneCap[ch]; 
      ADCInfo.nTelegraphMode                = m_pFH->nTelegraphMode[ch]; 
      ADCInfo.fTelegraphAccessResistance    = m_pFH->fTelegraphAccessResistance[ch]; 
      ADCInfo.nADCPtoLChannelMap            = m_pFH->nADCPtoLChannelMap[ch]; 
      ADCInfo.fADCProgrammableGain          = m_pFH->fADCProgrammableGain[ch]; 
      ADCInfo.fADCDisplayAmplification      = m_pFH->fADCDisplayAmplification[ch]; 
      ADCInfo.fADCDisplayOffset             = m_pFH->fADCDisplayOffset[ch]; 
      ADCInfo.fInstrumentScaleFactor        = m_pFH->fInstrumentScaleFactor[ch]; 
      ADCInfo.fInstrumentOffset             = m_pFH->fInstrumentOffset[ch]; 
      ADCInfo.fSignalGain                   = m_pFH->fSignalGain[ch]; 
      ADCInfo.fSignalOffset                 = m_pFH->fSignalOffset[ch]; 
      ADCInfo.fSignalLowpassFilter          = m_pFH->fSignalLowpassFilter[ch]; 
      ADCInfo.fSignalHighpassFilter         = m_pFH->fSignalHighpassFilter[ch]; 
      ADCInfo.nLowpassFilterType            = m_pFH->nLowpassFilterType[ch]; 
      ADCInfo.nHighpassFilterType           = m_pFH->nHighpassFilterType[ch]; 
      ADCInfo.fPostProcessLowpassFilter     = m_pFH->fPostProcessLowpassFilter[ch]; 
      ADCInfo.nPostProcessLowpassFilterType = m_pFH->nPostProcessLowpassFilterType[ch]; 
      ADCInfo.nStatsChannelPolarity         = m_pFH->nStatsChannelPolarity[ch]; 

      ADCInfo.lADCChannelNameIndex = ADD_STRING( m_pFH->sADCChannelName[ADCInfo.nADCNum] );
      ADCInfo.lADCUnitsIndex       = ADD_STRING( m_pFH->sADCUnits[ADCInfo.nADCNum] );
      
      m_FileInfo.ADCSection.Set( m_lNextBlock, sizeof( ADCInfo ), a+1 );
      m_pFI->Write( &ADCInfo, sizeof( ADCInfo ) );
   }
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return bOK;
}

//===============================================================================================
// FUNCTION: WriteDACInfo
// PURPOSE:  Writes the DAC info to the data file.
//
BOOL CProtocolWriterABF2::WriteDACInfo()
{
   MEMBERASSERT();

   BOOL bOK    = TRUE;
   UINT uCount = 1;
   ABF_DACInfo DACInfo;
   for( int d=0; d<ABF_DACCOUNT; d++ )
   {
      DACInfo.nDACNum                        = (short)d;
      DACInfo.nTelegraphDACScaleFactorEnable = m_pFH->nTelegraphDACScaleFactorEnable[d];
      DACInfo.fInstrumentHoldingLevel        = m_pFH->fInstrumentHoldingLevel[d];
      DACInfo.fDACScaleFactor                = m_pFH->fDACScaleFactor[d];
      DACInfo.fDACHoldingLevel               = m_pFH->fDACHoldingLevel[d];
      DACInfo.fDACCalibrationFactor          = m_pFH->fDACCalibrationFactor[d];
      DACInfo.fDACCalibrationOffset          = m_pFH->fDACCalibrationOffset[d];
      DACInfo.lDACFilePtr                    = m_pFH->lDACFilePtr[d];
      DACInfo.lDACFileNumEpisodes            = m_pFH->lDACFileNumEpisodes[d];
      DACInfo.nWaveformEnable                = m_pFH->nWaveformEnable[d];
      DACInfo.nWaveformSource                = m_pFH->nWaveformSource[d];
      DACInfo.nInterEpisodeLevel             = m_pFH->nInterEpisodeLevel[d];
      DACInfo.fDACFileScale                  = m_pFH->fDACFileScale[d];
      DACInfo.fDACFileOffset                 = m_pFH->fDACFileOffset[d];
      DACInfo.lDACFileEpisodeNum             = m_pFH->lDACFileEpisodeNum[d];
      DACInfo.nDACFileADCNum                 = m_pFH->nDACFileADCNum[d];
      DACInfo.nConditEnable                  = m_pFH->nConditEnable[d];
      DACInfo.lConditNumPulses               = m_pFH->lConditNumPulses[d];
      DACInfo.fBaselineDuration              = m_pFH->fBaselineDuration[d];
      DACInfo.fBaselineLevel                 = m_pFH->fBaselineLevel[d];
      DACInfo.fStepDuration                  = m_pFH->fStepDuration[d];
      DACInfo.fStepLevel                     = m_pFH->fStepLevel[d];
      DACInfo.fPostTrainPeriod               = m_pFH->fPostTrainPeriod[d];
      DACInfo.fPostTrainLevel                = m_pFH->fPostTrainLevel[d];
      DACInfo.nMembTestEnable                = m_pFH->nMembTestEnable[d];
      DACInfo.fMembTestPreSettlingTimeMS     = m_pFH->fMembTestPreSettlingTimeMS[d];
      DACInfo.fMembTestPostSettlingTimeMS    = m_pFH->fMembTestPostSettlingTimeMS[d];

      // Leak Subtraction.
      DACInfo.nLeakSubtractType              = m_pFH->nLeakSubtractType[d];
      DACInfo.nPNPosition                    = m_pFH->nPNPosition;
      DACInfo.nPNNumPulses                   = m_pFH->nPNNumPulses;
      DACInfo.fPNSettlingTime                = m_pFH->fPNSettlingTime;
      DACInfo.fPNInterpulse                  = m_pFH->fPNInterpulse;
      DACInfo.nPNPolarity                    = m_pFH->nPNPolarity;
      DACInfo.fPNHoldingLevel                = m_pFH->fPNHoldingLevel[d];
      DACInfo.nLeakSubtractADCIndex          = m_pFH->nLeakSubtractADCIndex[d];

      // LTP.
      DACInfo.nLTPUsageOfDAC                 = m_pFH->nLTPUsageOfDAC[d];
      DACInfo.nLTPPresynapticPulses          = m_pFH->nLTPPresynapticPulses[d];

      // Strings.
      DACInfo.lDACChannelNameIndex  = ADD_STRING( m_pFH->sDACChannelName[d] ); 
      DACInfo.lDACChannelUnitsIndex = ADD_STRING( m_pFH->sDACChannelUnits[d] );
      DACInfo.lDACFilePathIndex     = ADD_STRING( m_pFH->sDACFilePath[d] );
      
      m_FileInfo.DACSection.Set( m_lNextBlock, sizeof( DACInfo ), uCount++ );
      m_pFI->Write( &DACInfo, sizeof( DACInfo ) );
   }
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return bOK;
}

//===============================================================================================
// FUNCTION: WriteEpochs
// PURPOSE:  Writes the Epochs to the data file.
//
BOOL CProtocolWriterABF2::WriteEpochs()
{
   MEMBERASSERT();

   // Analog Epochs ... one set for each DAC in use.
   BOOL bOK        = TRUE;
   UINT uCount     = 1;
   bool bEpochInUse[ABF_EPOCHCOUNT];
   for( short e=0; e<ABF_EPOCHCOUNT; e++ )
      bEpochInUse[e] = FALSE;

   for( short d=0; d<ABF_DACCOUNT; d++ )
   {
      if( m_pFH->nOperationMode == ABF_WAVEFORMFILE && 
         ( m_pFH->nWaveformEnable[d] || m_pFH->nDigitalEnable ) )
      {
         for( short e=0; e<ABF_EPOCHCOUNT; e++ )
         {
            if( m_pFH->nEpochType[d][e] != ABF_EPOCHDISABLED )
            {
               ABF_EpochInfoPerDAC Epoch;

               bEpochInUse[e]           = TRUE;
               Epoch.nEpochNum          = e;
               Epoch.nDACNum            = d;
               Epoch.nEpochType         = m_pFH->nEpochType[d][e];
               Epoch.fEpochInitLevel    = m_pFH->fEpochInitLevel[d][e];
               Epoch.fEpochLevelInc     = m_pFH->fEpochLevelInc[d][e];
               Epoch.lEpochInitDuration = m_pFH->lEpochInitDuration[d][e];
               Epoch.lEpochDurationInc  = m_pFH->lEpochDurationInc[d][e];
               Epoch.lEpochPulsePeriod  = m_pFH->lEpochPulsePeriod[d][e];
               Epoch.lEpochPulseWidth   = m_pFH->lEpochPulseWidth[d][e];

               m_FileInfo.EpochPerDACSection.Set( m_lNextBlock, sizeof( Epoch ), uCount++ );
               m_pFI->Write( &Epoch, sizeof( Epoch ) );
            }
         }
      }
   }
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   // Digital Epochs ... one set only.
   if( m_FileInfo.EpochPerDACSection.llNumEntries )
   {
      uCount = 1;
      ABF_EpochInfo Epoch;
      for( short e=0; e<ABF_EPOCHCOUNT; e++ )
      {
         if( bEpochInUse[e] )
         {
            Epoch.nEpochNum                   = e;
            Epoch.nDigitalValue               = m_pFH->nDigitalValue[e];
            Epoch.nDigitalTrainValue          = m_pFH->nDigitalTrainValue[e];
            Epoch.nAlternateDigitalValue      = m_pFH->nAlternateDigitalValue[e];
            Epoch.nAlternateDigitalTrainValue = m_pFH->nAlternateDigitalTrainValue[e];
            Epoch.bEpochCompression           = m_pFH->bEpochCompression[e];

            m_FileInfo.EpochSection.Set( m_lNextBlock, sizeof( Epoch ), uCount++ );
            m_pFI->Write( &Epoch, sizeof( Epoch ) );
         }
      }
   }
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return bOK;
}

//===============================================================================================
// FUNCTION: WriteStats
// PURPOSE:  Writes the Stats regions to the data file.
//
BOOL CProtocolWriterABF2::WriteStats()
{
   MEMBERASSERT();

   // Stats regions.
   BOOL bOK    = TRUE;
   UINT uCount = 1;
   ABF_StatsRegionInfo Stats;
   for( short r=0; r<ABF_STATS_REGIONS; r++ )
   {
      UINT uBitMask = 0x01 << r;
      if( m_pFH->nStatsSearchRegionFlags & uBitMask )
      {
         Stats.nRegionNum              = r;
         Stats.lStatsMeasurements      = m_pFH->lStatsMeasurements[r];
         Stats.lStatsStart             = m_pFH->lStatsStart[r];
         Stats.lStatsEnd               = m_pFH->lStatsEnd[r];
         Stats.nRiseTopPercentile      = m_pFH->nRiseTopPercentile[r];
         Stats.nRiseBottomPercentile   = m_pFH->nRiseBottomPercentile[r];
         Stats.nDecayBottomPercentile  = m_pFH->nDecayBottomPercentile[r];
         Stats.nDecayTopPercentile     = m_pFH->nDecayTopPercentile[r];
         Stats.nStatsSearchMode        = m_pFH->nStatsSearchMode[r];
         Stats.nStatsSearchDAC         = m_pFH->nStatsSearchDAC[r];

         Stats.nStatsActiveChannels    = m_pFH->nStatsActiveChannels;
         Stats.nStatsSearchRegionFlags = m_pFH->nStatsSearchRegionFlags;
         Stats.nStatsSmoothing         = m_pFH->nStatsSmoothing;
         Stats.nStatsSmoothingEnable   = m_pFH->nStatsSmoothingEnable;
         Stats.nStatsBaseline          = m_pFH->nStatsBaseline;
         Stats.nStatsBaselineDAC       = m_pFH->nStatsBaselineDAC;
         Stats.lStatsBaselineStart     = m_pFH->lStatsBaselineStart;
         Stats.lStatsBaselineEnd       = m_pFH->lStatsBaselineEnd;

         m_FileInfo.StatsRegionSection.Set( m_lNextBlock, sizeof( Stats ), uCount++ );
         m_pFI->Write( &Stats, sizeof( Stats ) );
      }
   }
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return bOK;
}

//===============================================================================================
// FUNCTION: WriteUserList
// PURPOSE:  Writes the user list to the data file.
//
BOOL CProtocolWriterABF2::WriteUserList()
{
   MEMBERASSERT();

   BOOL bOK    = TRUE;
   UINT uCount = 1;
   ABF_UserListInfo UserList;
   for( short u=0; u<ABF_DACCOUNT; u++ )
   {
      if( m_pFH->nULEnable[u] )
      {
         UserList.nListNum       = u;
         UserList.nULParamToVary = m_pFH->nULParamToVary[u];
         UserList.nULRepeat      = m_pFH->nULRepeat[u];

         UserList.lULParamValueListIndex = ADD_STRING( m_pFH->sULParamValueList[u] );

         m_FileInfo.UserListSection.Set( m_lNextBlock, sizeof( UserList ), uCount++ );
         bOK &= m_pFI->Write( &UserList, sizeof( UserList ) );
      }
   }
   bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );

   return bOK;
}

//===============================================================================================
// FUNCTION: WriteMathInfo
// PURPOSE:  Writes the math channel info to the data file.
// NOTES:    We currently only support one math channel, but the file can support any number.
//
BOOL CProtocolWriterABF2::WriteMathInfo()
{
   MEMBERASSERT();

   BOOL bOK    = TRUE;
   ABF_MathInfo Math;
   if( m_pFH->nArithmeticEnable )
   {
      Math.nMathEnable     = m_pFH->nArithmeticEnable;

      Math.nMathExpression = m_pFH->nArithmeticExpression;   
      Math.fMathUpperLimit = m_pFH->fArithmeticUpperLimit;   
      Math.fMathLowerLimit = m_pFH->fArithmeticLowerLimit;  

      Math.nMathADCNum[0]  = m_pFH->nArithmeticADCNumA;   
      Math.nMathADCNum[1]  = m_pFH->nArithmeticADCNumB;   

      Math.fMathK[0]       = m_pFH->fArithmeticK1;     
      Math.fMathK[1]       = m_pFH->fArithmeticK2;     
      Math.fMathK[2]       = m_pFH->fArithmeticK3;     
      Math.fMathK[3]       = m_pFH->fArithmeticK4;     
      Math.fMathK[4]       = m_pFH->fArithmeticK5;     
      Math.fMathK[5]       = m_pFH->fArithmeticK6;     

      Math.uMathOperatorIndex = ADD_STRING( m_pFH->sArithmeticOperator );
      Math.uMathUnitsIndex    = ADD_STRING( m_pFH->sArithmeticUnits );

      m_FileInfo.MathSection.Set( m_lNextBlock, sizeof( ABF_MathInfo ), 1 );
      bOK &= m_pFI->Write( &Math, sizeof( Math ) );
      bOK &= m_pFI->FillCurrentBlock( &m_lNextBlock );
   }

   return bOK;
}
