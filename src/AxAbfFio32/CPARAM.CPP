//***********************************************************************************************
//
//    Copyright (c) 1993-2002 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// This is CPARAM.C; A module of HEADER.EXE for setting/showing specific
//                   parameters from an ABF file header.
// 
// An ANSI C compiler should be used for compilation.
// Compile with the large memory model option.
// (e.g. CL -c -AL ATFTST.C ATFFILES.C)

#include "wincpp.hpp"

#include "cparam.h"
#include "abffiles.h"
#include "abfheadr.h"

static char *CP_Errors[] = 
{   
   "",
   "Cannot open file",
   "Cannot read from file",
   "Cannot seek",
   "Cannot write to file",
   "Parameter not found",
   "Invalid file. Only ABF format supported",
   "Index out of range"
};

#define  PARAM_STRING    1
#define  PARAM_SHORT     2
#define  PARAM_LONG      3
#define  PARAM_FLOAT     4
#define  PARAM_BYTE      5
#define  PARAM_DOUBLE    6

#define  ABFNAME(i)        (ABFParameters[i].Name)
#define  ABFTYPE(i)        (ABFParameters[i].Type)
#define  ABFNUMARGS(i)     (ABFParameters[i].NumArgs)
#define  ABFSIZE(i)        (ABFParameters[i].Size)
#define  ABFADDRESS(i)     (ABFParameters[i].Address)
#define  ABFSVALUE(i)      (((char*)ABFParameters[i].Address+ABFSIZE(i)*CPIndex))
#define  ABFNVALUE(i)      (*((short*)ABFParameters[i].Address+CPIndex))
#define  ABFLVALUE(i)      (*((long*)ABFParameters[i].Address+CPIndex))
#define  ABFFVALUE(i)      (*((float*)ABFParameters[i].Address+CPIndex))
#define  ABFDVALUE(i)      (*((double*)ABFParameters[i].Address+CPIndex))

static void SetParam(int i, int CPIndex, char *pszVal);
static void ShowParam(int i, int CPIndex, int bNewValue,
                      int bShowIndexAsChar);

typedef struct
{   
   char     *Name;         // eg. lActualEpisodes
   void     *Address;      // eg. &pFH->lActualEpisodes
   int      Type;          // eg. PARAM_LONG
   int      NumArgs;       // eg. DAC_COUNT
   int      Size;          // eg. FILETYPELEN
} ABFParameter;

static ABFFileHeader ABFFH;

static ABFParameter ABFParameters[] =
{   
   // GROUP #1 (40 bytes)

   {"lFileSignature",            &ABFFH.lFileSignature,
                                 PARAM_LONG, 1, sizeof(long)},

   {"fFileVersionNumber",        &ABFFH.fFileVersionNumber,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nOperationMode",            &ABFFH.nOperationMode,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lActualAcqLength",          &ABFFH.lActualAcqLength,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nNumPointsIgnored",         &ABFFH.nNumPointsIgnored,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lActualEpisodes",           &ABFFH.lActualEpisodes,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lFileStartDate",            &ABFFH.lFileStartDate,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lFileStartTime",            &ABFFH.lFileStartTime,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lStopwatchTime",            &ABFFH.lStopwatchTime,
                                 PARAM_LONG, 1, sizeof(long)},

   {"fHeaderVersionNumber",      &ABFFH.fHeaderVersionNumber,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nFileType",                 &ABFFH.nFileType,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nMSBinFormat",              &ABFFH.nMSBinFormat,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #2 (80 bytes)

   {"lDataSectionPtr",           &ABFFH.lDataSectionPtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lTagSectionPtr",            &ABFFH.lTagSectionPtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lNumTagEntries",            &ABFFH.lNumTagEntries,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lScopeConfigPtr",           &ABFFH.lScopeConfigPtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lNumScopes",                &ABFFH.lNumScopes,
                                 PARAM_LONG, 1, sizeof(long)},

   {"_lDACFilePtr",              &ABFFH._lDACFilePtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"_lDACFileNumEpisodes",      &ABFFH._lDACFileNumEpisodes,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lDeltaArrayPtr",            &ABFFH.lDeltaArrayPtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lNumDeltas",                &ABFFH.lNumDeltas,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lVoiceTagPtr",              &ABFFH.lVoiceTagPtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lVoiceTagEntries",          &ABFFH.lVoiceTagEntries,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lSynchArrayPtr",            &ABFFH.lSynchArrayPtr,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lSynchArraySize",           &ABFFH.lSynchArraySize,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nDataFormat",               &ABFFH.nDataFormat,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nSimultaneousScan",         &ABFFH.nSimultaneousScan,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lStatisticsConfigPtr",      &ABFFH.lStatisticsConfigPtr,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #3 (80 bytes)

   {"channel_count_acquired",    &ABFFH.channel_count_acquired,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nADCNumChannels",           &ABFFH.nADCNumChannels,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fADCSampleInterval",        &ABFFH.fADCSampleInterval,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fADCSecondSampleInterval",  &ABFFH.fADCSecondSampleInterval,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fSynchTimeUnit",            &ABFFH.fSynchTimeUnit,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fSecondsPerRun",            &ABFFH.fSecondsPerRun,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"lNumSamplesPerEpisode",     &ABFFH.lNumSamplesPerEpisode,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lPreTriggerSamples",        &ABFFH.lPreTriggerSamples,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lEpisodesPerRun",           &ABFFH.lEpisodesPerRun,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lRunsPerTrial",             &ABFFH.lRunsPerTrial,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lNumberOfTrials",           &ABFFH.lNumberOfTrials,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nAveragingMode",            &ABFFH.nAveragingMode,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nUndoRunCount",             &ABFFH.nUndoRunCount,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nFirstEpisodeInRun",        &ABFFH.nFirstEpisodeInRun,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fTriggerThreshold",         &ABFFH.fTriggerThreshold,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nTriggerSource",            &ABFFH.nTriggerSource,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nTriggerAction",            &ABFFH.nTriggerAction,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nTriggerPolarity",          &ABFFH.nTriggerPolarity,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fScopeOutputInterval",      &ABFFH.fScopeOutputInterval,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fEpisodeStartToStart",      &ABFFH.fEpisodeStartToStart,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fRunStartToStart",          &ABFFH.fRunStartToStart,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fTrialStartToStart",        &ABFFH.fTrialStartToStart,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"lAverageCount",             &ABFFH.lAverageCount,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lClockChange",              &ABFFH.lClockChange,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nAutoTriggerStrategy",      &ABFFH.nAutoTriggerStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #4 (44 bytes)

   {"nDrawingStrategy",          &ABFFH.nDrawingStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nTiledDisplay",             &ABFFH.nTiledDisplay,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nEraseStrategy",            &ABFFH.nEraseStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nDataDisplayMode",          &ABFFH.nDataDisplayMode,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lDisplayAverageUpdate",     &ABFFH.lDisplayAverageUpdate,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nChannelStatsStrategy",     &ABFFH.nChannelStatsStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lCalculationPeriod",        &ABFFH.lCalculationPeriod,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lSamplesPerTrace",          &ABFFH.lSamplesPerTrace,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lStartDisplayNum",          &ABFFH.lStartDisplayNum,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lFinishDisplayNum",         &ABFFH.lFinishDisplayNum,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nMultiColor",               &ABFFH.nMultiColor,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nShowPNRawData",            &ABFFH.nShowPNRawData,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fStatisticsPeriod",         &ABFFH.fStatisticsPeriod,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"lStatisticsMeasurements",   &ABFFH.lStatisticsMeasurements,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nStatisticsSaveStrategy",   &ABFFH.nStatisticsSaveStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #5 (16 bytes)

   {"fADCRange",                 &ABFFH.fADCRange,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fDACRange",                 &ABFFH.fDACRange,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"lADCResolution",            &ABFFH.lADCResolution,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lDACResolution",            &ABFFH.lDACResolution,
                                 PARAM_LONG, 1, sizeof(long)},

   // GROUP #6 (118 bytes)

   {"nExperimentType",           &ABFFH.nExperimentType,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutosampleEnable",        &ABFFH._nAutosampleEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutosampleADCNum",        &ABFFH._nAutosampleADCNum,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutosampleInstrument",    &ABFFH._nAutosampleInstrument,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_fAutosampleAdditGain",     &ABFFH._fAutosampleAdditGain,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fAutosampleFilter",        &ABFFH._fAutosampleFilter,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fAutosampleMembraneCap",   &ABFFH._fAutosampleMembraneCap,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nManualInfoStrategy",       &ABFFH.nManualInfoStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fCellID1",                  &ABFFH.fCellID1,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fCellID2",                  &ABFFH.fCellID2,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fCellID3",                  &ABFFH.fCellID3,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"sCreatorInfo",              &ABFFH.sCreatorInfo[0],
                                 PARAM_STRING, 1, ABF_CREATORINFOLEN},

   {"_sFileComment",             &ABFFH._sFileComment[0],
                                 PARAM_STRING, 1, ABF_FILECOMMENTLEN},
   
   {"nFileStartMillisecs",       &ABFFH.nFileStartMillisecs,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nCommentsEnable",           &ABFFH.nCommentsEnable,
                                 PARAM_SHORT, 1, sizeof(short)},


   // GROUP #7 (1044 (160 + 384 + 488 + 12) bytes) - Multi-channel information.

   {"nADCPtoLChannelMap",        &ABFFH.nADCPtoLChannelMap,
                                 PARAM_SHORT, ABF_ADCCOUNT, sizeof(short)},

   {"nADCSamplingSeq",           &ABFFH.nADCSamplingSeq,
                                 PARAM_SHORT, ABF_ADCCOUNT, sizeof(short)},

   {"sADCChannelName",           &ABFFH.sADCChannelName[0],
                                 PARAM_STRING, ABF_ADCCOUNT, ABF_ADCNAMELEN},

   {"sADCUnits",                 &ABFFH.sADCUnits[0],
                                 PARAM_STRING, ABF_ADCCOUNT, ABF_ADCUNITLEN},

   {"fADCProgrammableGain",      &ABFFH.fADCProgrammableGain,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fADCDisplayAmplification",  &ABFFH.fADCDisplayAmplification,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fADCDisplayOffset",         &ABFFH.fADCDisplayOffset,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fInstrumentScaleFactor",    &ABFFH.fInstrumentScaleFactor,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fInstrumentOffset",         &ABFFH.fInstrumentOffset,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fSignalGain",               &ABFFH.fSignalGain,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fSignalOffset",             &ABFFH.fSignalOffset,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fSignalLowpassFilter",      &ABFFH.fSignalLowpassFilter,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fSignalHighpassFilter",     &ABFFH.fSignalHighpassFilter,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"sDACChannelName",           &ABFFH.sDACChannelName[0],
                                 PARAM_STRING, ABF_DACCOUNT, ABF_DACNAMELEN},

   {"sDACChannelUnits",          &ABFFH.sDACChannelUnits[0],
                                 PARAM_STRING, ABF_DACCOUNT, ABF_DACUNITLEN},

   {"fDACScaleFactor",           &ABFFH.fDACScaleFactor,
                                 PARAM_FLOAT, ABF_DACCOUNT, sizeof(float)},

   {"fDACHoldingLevel",          &ABFFH.fDACHoldingLevel,
                                 PARAM_FLOAT, ABF_DACCOUNT, sizeof(float)},

   {"nSignalType",               &ABFFH.nSignalType,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #8 (14 bytes) - Synchronous timer outputs.

   {"nOUTEnable",                &ABFFH.nOUTEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nSampleNumberOUT1",         &ABFFH.nSampleNumberOUT1,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nSampleNumberOUT2",         &ABFFH.nSampleNumberOUT2,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nFirstEpisodeOUT",          &ABFFH.nFirstEpisodeOUT,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nLastEpisodeOUT",           &ABFFH.nLastEpisodeOUT,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nPulseSamplesOUT1",         &ABFFH.nPulseSamplesOUT1,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nPulseSamplesOUT2",         &ABFFH.nPulseSamplesOUT2,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #9 (184 bytes) - Epoch Output Waveform and Pulses;

   {"nDigitalEnable",            &ABFFH.nDigitalEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nWaveformSource",          &ABFFH._nWaveformSource,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nActiveDACChannel",         &ABFFH.nActiveDACChannel,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nInterEpisodeLevel",        &ABFFH._nInterEpisodeLevel,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nEpochType",                &ABFFH._nEpochType,
                                 PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(short)},

   {"_fEpochInitLevel",           &ABFFH._fEpochInitLevel,
                                 PARAM_FLOAT, ABF_EPOCHCOUNT, sizeof(float)},

   {"_fEpochLevelInc",            &ABFFH._fEpochLevelInc,
                                 PARAM_FLOAT, ABF_EPOCHCOUNT, sizeof(float)},

   {"_nEpochInitDuration",        &ABFFH._nEpochInitDuration,
                                 PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(long)},

   {"_nEpochDurationInc",         &ABFFH._nEpochDurationInc,
                                 PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(long)},

   {"nDigitalHolding",           &ABFFH.nDigitalHolding,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nDigitalInterEpisode",      &ABFFH.nDigitalInterEpisode,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nDigitalValue",             &ABFFH.nDigitalValue,
                                 PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(short)},
   
   {"nDigitalDACChannel",        &ABFFH.nDigitalDACChannel,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #10 (98 bytes) - Analog Output File Waveform

   {"_fDACFileScale",            &ABFFH._fDACFileScale,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fDACFileOffset",           &ABFFH._fDACFileOffset,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_nDACFileEpisodeNum",       &ABFFH._nDACFileEpisodeNum,
                                 PARAM_SHORT, 1, sizeof(long)},

   {"_nDACFileADCNum",           &ABFFH._nDACFileADCNum,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_sDACFilePath",             &ABFFH._sDACFilePath[0],
                                 PARAM_STRING, 1, ABF_DACFILEPATHLEN},

   // GROUP #11 (44 bytes) - Presweep (conditioning) pulse train.

   {"_nConditEnable",            &ABFFH._nConditEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nConditChannel",           &ABFFH._nConditChannel,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_lConditNumPulses",         &ABFFH._lConditNumPulses,
                                 PARAM_LONG, 1, sizeof(long)},

   {"_fBaselineDuration",        &ABFFH._fBaselineDuration,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fBaselineLevel",           &ABFFH._fBaselineLevel,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fStepDuration",            &ABFFH._fStepDuration,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fStepLevel",               &ABFFH._fStepLevel,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fPostTrainPeriod",         &ABFFH._fPostTrainPeriod,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"_fPostTrainLevel",          &ABFFH._fPostTrainLevel,
                                 PARAM_FLOAT, 1, sizeof(float)},

   // GROUP #12 ( 82 bytes) - Variable parameter list.

   {"_nParamToVary",             &ABFFH._nParamToVary,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_sParamValueList",          &ABFFH._sParamValueList[0],
                                 PARAM_STRING, 1, ABF_VARPARAMLISTLEN},

   // GROUP #13 (36 bytes) - Autopeak measurement.

   {"_nAutopeakEnable",          &ABFFH._nAutopeakEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutopeakPolarity",        &ABFFH._nAutopeakPolarity,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutopeakADCNum",          &ABFFH._nAutopeakADCNum,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutopeakSearchMode",      &ABFFH._nAutopeakSearchMode,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_lAutopeakStart",           &ABFFH._lAutopeakStart,
                                 PARAM_LONG, 1, sizeof(long)},

   {"_lAutopeakEnd",             &ABFFH._lAutopeakEnd,
                                 PARAM_LONG, 1, sizeof(long)},

   {"_nAutopeakSmoothing",       &ABFFH._nAutopeakSmoothing,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutopeakBaseline",        &ABFFH._nAutopeakBaseline,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nAutopeakAverage",         &ABFFH._nAutopeakAverage,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_lAutopeakMeasurements",    &ABFFH._lAutopeakMeasurements,
                                 PARAM_LONG, 1, sizeof(long)},

   // GROUP #14 (52 bytes) - Channel Arithmetic

   {"nArithmeticEnable",         &ABFFH.nArithmeticEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fArithmeticUpperLimit",     &ABFFH.fArithmeticUpperLimit,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fArithmeticLowerLimit",     &ABFFH.fArithmeticLowerLimit,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nArithmeticADCNumA",        &ABFFH.nArithmeticADCNumA,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nArithmeticADCNumB",        &ABFFH.nArithmeticADCNumB,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fArithmeticK1",             &ABFFH.fArithmeticK1,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fArithmeticK2",             &ABFFH.fArithmeticK2,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fArithmeticK3",             &ABFFH.fArithmeticK3,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fArithmeticK4",             &ABFFH.fArithmeticK4,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"sArithmeticOperator",       &ABFFH.sArithmeticOperator[0],
                                 PARAM_STRING, 1, ABF_ARITHMETICOPLEN},

   {"sArithmeticUnits",          &ABFFH.sArithmeticUnits[0],
                                 PARAM_STRING, 1, ABF_ARITHMETICUNITSLEN},

   {"fArithmeticK5",             &ABFFH.fArithmeticK5,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fArithmeticK6",             &ABFFH.fArithmeticK6,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nArithmeticExpression",     &ABFFH.nArithmeticExpression,
                                 PARAM_SHORT, 1, sizeof(short)},

   // GROUP #15 (34 bytes) - On-line subtraction.

   {"_nPNEnable",                &ABFFH._nPNEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nPNPosition",               &ABFFH.nPNPosition,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nPNPolarity",              &ABFFH._nPNPolarity,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nPNNumPulses",              &ABFFH.nPNNumPulses,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nPNADCNum",                &ABFFH._nPNADCNum,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_fPNHoldingLevel",          &ABFFH._fPNHoldingLevel,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fPNSettlingTime",           &ABFFH.fPNSettlingTime,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"fPNInterpulse",             &ABFFH.fPNInterpulse,
                                 PARAM_FLOAT, 1, sizeof(float)},

   // GROUP #16 (54 bytes) - Unused space at end of header block.

   {"_nListEnable",              &ABFFH._nListEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nBellEnable",               &ABFFH.nBellEnable,
                                 PARAM_SHORT, ABF_BELLCOUNT, sizeof(short)},

   {"nBellLocation",             &ABFFH.nBellLocation,
                                 PARAM_SHORT, ABF_BELLCOUNT, sizeof(short)},

   {"nBellRepetitions",          &ABFFH.nBellRepetitions,
                                 PARAM_SHORT, ABF_BELLCOUNT, sizeof(short)},

   {"nLevelHysteresis",          &ABFFH.nLevelHysteresis,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lTimeHysteresis",           &ABFFH.lTimeHysteresis,
                                 PARAM_LONG, 1, sizeof(long)},

   {"nAllowExternalTags",        &ABFFH.nAllowExternalTags,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nLowpassFilterType",        &ABFFH.nLowpassFilterType,
                                 PARAM_BYTE, ABF_ADCCOUNT, sizeof(char)},

   {"nHighpassFilterType",       &ABFFH.nHighpassFilterType,
                                 PARAM_BYTE, ABF_ADCCOUNT, sizeof(char)},

   {"nAverageAlgorithm",         &ABFFH.nAverageAlgorithm,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"fAverageWeighting",         &ABFFH.fAverageWeighting,
                                 PARAM_FLOAT, 1, sizeof(float)},

   {"nUndoPromptStrategy",       &ABFFH.nUndoPromptStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nTrialTriggerSource",       &ABFFH.nTrialTriggerSource,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatisticsDisplayStrategy", &ABFFH.nStatisticsDisplayStrategy,
                                  PARAM_SHORT, 1, sizeof(short)},

   {"nExternalTagType",          &ABFFH.nExternalTagType,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lHeaderSize",               &ABFFH.lHeaderSize,
                                 PARAM_LONG, 1, sizeof(long)},

   {"dFileDuration",             &ABFFH.dFileDuration,
                                 PARAM_DOUBLE, 1, sizeof(long)},

   {"nStatisticsClearStrategy",  &ABFFH.nStatisticsClearStrategy,
                                 PARAM_SHORT, 1, sizeof(short)},

   // EXTENDED GROUP #2 - File Structure (8 * 2 = 16 + 100 bytes)
                                 
   {"lDACFilePtr",               &ABFFH.lDACFilePtr,
                                 PARAM_LONG, ABF_WAVEFORMCOUNT, sizeof(long)},

   {"lDACFileNumEpisodes",       &ABFFH.lDACFileNumEpisodes,
                                 PARAM_LONG, ABF_WAVEFORMCOUNT, sizeof(long)},

   // EXTENDED GROUP #7 - Multi-channel information (8 * 4 = 32 + 100 bytes)
   
   {"fDACCalibrationFactor",     &ABFFH.fDACCalibrationFactor,
                                 PARAM_FLOAT, ABF_DACCOUNT, sizeof(float)},
                                 
   {"fDACCalibrationOffset",     &ABFFH.fDACCalibrationOffset,
                                 PARAM_FLOAT, ABF_DACCOUNT, sizeof(float)},

   // Train paramters

   {"lEpochPulsePeriod",         &ABFFH.lEpochPulsePeriod,
                                 PARAM_LONG, ABF_EPOCHCOUNT, ABF_WAVEFORMCOUNT},

   {"lEpochPulseWidth",          &ABFFH.lEpochPulseWidth,
                                 PARAM_LONG, ABF_EPOCHCOUNT, ABF_WAVEFORMCOUNT},

   // EXTENDED GROUP #9 - Epoch Waveform and Pulses ( 186 * 2 = 368 + 100 bytes)

   {"nWaveformEnable",           &ABFFH.nWaveformEnable,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   {"nWaveformSource",           &ABFFH.nWaveformSource,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},
                                 
   {"nInterEpisodeLevel",        &ABFFH.nInterEpisodeLevel,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},
   
   {"nEpochType",                &ABFFH.nEpochType,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, ABF_WAVEFORMCOUNT},

   {"fEpochInitLevel",           &ABFFH.fEpochInitLevel,
                                 PARAM_FLOAT, ABF_EPOCHCOUNT, ABF_WAVEFORMCOUNT},

   {"fEpochLevelInc",            &ABFFH.fEpochLevelInc,
                                 PARAM_FLOAT, ABF_EPOCHCOUNT, ABF_WAVEFORMCOUNT},

   {"lEpochInitDuration",        &ABFFH.lEpochInitDuration,
                                 PARAM_LONG, ABF_EPOCHCOUNT, ABF_WAVEFORMCOUNT},

   {"lEpochDurationInc",         &ABFFH.lEpochDurationInc,
                                 PARAM_LONG, ABF_EPOCHCOUNT, ABF_WAVEFORMCOUNT},

   {"nDigitalTrainValue",        &ABFFH.nDigitalTrainValue,
                                 PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(short)},

   {"nDigitalTrainActiveLogic",  &ABFFH.nDigitalTrainActiveLogic,
                                 PARAM_SHORT, 1, sizeof(short)},


   // EXTENDED GROUP #10 - DAC Output File (270 * 2 = 540 + 12 bytes)

   {"fDACFileScale",             &ABFFH.fDACFileScale,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"fDACFileOffset",            &ABFFH.fDACFileOffset,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"lDACFileEpisodeNum",        &ABFFH.lDACFileEpisodeNum,
                                 PARAM_LONG, ABF_WAVEFORMCOUNT, sizeof(long)},

   {"nDACFileADCNum",            &ABFFH.nDACFileADCNum,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   {"sDACFilePath",              &ABFFH.sDACFilePath[0],
                                 PARAM_STRING, ABF_WAVEFORMCOUNT, ABF_PATHLEN},

   // EXTENDED GROUP #11 - Presweep (conditioning) pulse train (2 * 32 = 64 + 36 bytes)

   {"nConditEnable",             &ABFFH.nConditEnable,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   {"lConditNumPulses",          &ABFFH.lConditNumPulses,
                                 PARAM_LONG, ABF_WAVEFORMCOUNT, sizeof(long)},

   {"fBaselineDuration",         &ABFFH.fBaselineDuration,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"fBaselineLevel",            &ABFFH.fBaselineLevel,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"fStepDuration",             &ABFFH.fStepDuration,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"fStepLevel",                &ABFFH.fStepLevel,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"fPostTrainPeriod",          &ABFFH.fPostTrainPeriod,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   {"fPostTrainLevel",           &ABFFH.fPostTrainLevel,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   // EXTENDED GROUP #12 - Variable parameter user list (260 * 4 = 1040 + 56 bytes)

   {"nULEnable",                 &ABFFH.nULEnable,
                                 PARAM_SHORT, ABF_USERLISTCOUNT, sizeof(short)},

   {"nULParamToVary",            &ABFFH.nULParamToVary,
                                 PARAM_SHORT, ABF_USERLISTCOUNT, sizeof(short)},

   {"sULParamValueList",         &ABFFH.sULParamValueList[0],
                                 PARAM_STRING, ABF_USERLISTCOUNT, ABF_USERLISTLEN},

   {"nULRepeat",                 &ABFFH.nULRepeat,
                                 PARAM_SHORT, ABF_USERLISTCOUNT, sizeof(short)},

   // EXTENDED GROUP #15 - On-line subtraction (10 * 2 = 20 + 36 bytes)

   {"nPNEnable",                 &ABFFH.nPNEnable,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   {"nPNPolarity",               &ABFFH.nPNPolarity,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   {"__nPNADCNum",               &ABFFH.__nPNADCNum,
                                 PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   {"fPNHoldingLevel",           &ABFFH.fPNHoldingLevel,
                                 PARAM_FLOAT, ABF_WAVEFORMCOUNT, sizeof(float)},

   // EXTENDED GROUP #6 Environmental Information  ( (16 * 20) + 128 = 448 + 128 bytes)
   
   {"nTelegraphEnable",          &ABFFH.nTelegraphEnable,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"nTelegraphInstrument",      &ABFFH.nTelegraphInstrument,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fTelegraphAdditGain",       &ABFFH.fTelegraphAdditGain,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fTelegraphFilter",          &ABFFH.fTelegraphFilter,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"fTelegraphMembraneCap",     &ABFFH.fTelegraphMembraneCap,
                                 PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"nTelegraphMode",            &ABFFH.nTelegraphMode,
                                 PARAM_SHORT, ABF_ADCCOUNT, sizeof(short)},

   {"nTelegraphDACScaleFactorEnable", &ABFFH.nTelegraphDACScaleFactorEnable,
                                 PARAM_SHORT, ABF_DACCOUNT, sizeof(short)},

   {"nAutoAnalyseEnable",        &ABFFH.nAutoAnalyseEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"sAutoAnalysisMacroName",    &ABFFH.sAutoAnalysisMacroName[0],
                                 PARAM_STRING, 1, ABF_MACRONAMELEN},

   {"sProtocolPath",             &ABFFH.sProtocolPath[0],
                                 PARAM_STRING, 1, ABF_PATHLEN},

   {"sFileComment",              &ABFFH.sFileComment[0],
                                 PARAM_STRING, 1, ABF_FILECOMMENTLEN},

   {"fInstrumentHoldingLevel",   &ABFFH.fInstrumentHoldingLevel[0],
                                 PARAM_FLOAT, 1, ABF_DACCOUNT},

   {"ulFileCRC",                 &ABFFH.ulFileCRC,
                                 PARAM_LONG, 1, sizeof(unsigned short)},

   {"sModifierInfo",             &ABFFH.sModifierInfo[0],
                                 PARAM_STRING, 1, ABF_FILECOMMENTLEN},

   // EXTENDED GROUP #13 - Statistics measurements ( 216 + 188 = 404 bytes)

   {"nStatsEnable",              &ABFFH.nStatsEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatsActiveChannels",      &ABFFH.nStatsActiveChannels,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatsSearchRegionFlags",   &ABFFH.nStatsSearchRegionFlags,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatsSelectedRegion",      &ABFFH.nStatsSelectedRegion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"_nStatsSearchMode",         &ABFFH._nStatsSearchMode,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatsSmoothing",           &ABFFH.nStatsSmoothing,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatsSmoothingEnable",     &ABFFH.nStatsSmoothingEnable,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nStatsBaseline",            &ABFFH.nStatsBaseline,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"lStatsBaselineStart",       &ABFFH.lStatsBaselineStart,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lStatsBaselineEnd",         &ABFFH.lStatsBaselineEnd,
                                 PARAM_LONG, 1, sizeof(long)},

   {"lStatsMeasurements",        &ABFFH.lStatsMeasurements,
                                 PARAM_LONG, ABF_STATS_REGIONS, sizeof(long)},

   {"lStatsStart",               &ABFFH.lStatsStart,
                                 PARAM_LONG, ABF_STATS_REGIONS, sizeof(long)},

   {"lStatsEnd",                 &ABFFH.lStatsEnd,
                                 PARAM_LONG, ABF_STATS_REGIONS, sizeof(long)},

   {"nRiseBottomPercentile",     &ABFFH.nRiseBottomPercentile,
                                 PARAM_SHORT, ABF_STATS_REGIONS, sizeof(short)},

   {"nRiseTopPercentile",        &ABFFH.nRiseTopPercentile,
                                 PARAM_SHORT, ABF_STATS_REGIONS, sizeof(short)},

   {"nDecayBottomPercentile",    &ABFFH.nDecayBottomPercentile,
                                 PARAM_SHORT, ABF_STATS_REGIONS, sizeof(short)},

   {"nDecayTopPercentile",       &ABFFH.nDecayTopPercentile,
                                 PARAM_SHORT, ABF_STATS_REGIONS, sizeof(short)},

   {"nStatsChannelPolarity",     &ABFFH.nStatsChannelPolarity,
                                 PARAM_SHORT, ABF_ADCCOUNT, sizeof(short)},

   {"nStatsSearchMode",          &ABFFH.nStatsSearchMode,
                                 PARAM_SHORT, ABF_STATS_REGIONS, sizeof(short)},

   // Group #17 - Application version data ( 2 * 4 + 8 = 16 bytes )

   {"nCreatorMajorVersion",      &ABFFH.nCreatorMajorVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nCreatorMinorVersion",      &ABFFH.nCreatorMinorVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nCreatorBugfixVersion",     &ABFFH.nCreatorBugfixVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nCreatorBuildVersion",      &ABFFH.nCreatorBuildVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nModifierMajorVersion",     &ABFFH.nModifierMajorVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nModifierMinorVersion",      &ABFFH.nModifierMinorVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nModifierBugfixVersion",    &ABFFH.nModifierBugfixVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nModifierBuildVersion",     &ABFFH.nModifierBuildVersion,
                                 PARAM_SHORT, 1, sizeof(short)},

   // Group #18 - LTP protocol ( 2 + ( 4 * 2 ) + 4 = 14 bytes )

   {"nLTPType",                  &ABFFH.nLTPType,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nLTPUsageOfDAC",            &ABFFH.nLTPUsageOfDAC,
                                 PARAM_SHORT, 1, sizeof(short)},

   {"nLTPPresynapticPulses",     &ABFFH.nLTPPresynapticPulses,
                                 PARAM_SHORT, 1, sizeof(short)},

   // Group #19 - Digidata 132x Trigger out flag. ( 2 + 6 = 8 bytes )

   {"nDD132xTriggerOut",         &ABFFH.nDD132xTriggerOut,
                                 PARAM_SHORT, 1, sizeof(short)},

   // Group #20 - Epoch resistance ( ( 2 * 10 ) + ( 2 * 2 ) + 32 = 56 bytes )

   {"sEpochResistanceSignalName", &ABFFH.sEpochResistanceSignalName[0],
                                  PARAM_STRING, ABF_WAVEFORMCOUNT, ABF_ADCNAMELEN},

   {"nEpochResistanceState",      &ABFFH.nEpochResistanceState,
                                  PARAM_SHORT, ABF_WAVEFORMCOUNT, sizeof(short)},

   
   // Group #21 - Alternating episodic mode ( ( 2 * 10 * 2 ) + 2 + 32 = 104 bytes )
   
   {"nAlternateDACOutputState",     &ABFFH.nAlternateDACOutputState,
                                    PARAM_SHORT, 1, sizeof(short)},

   {"nAlternateDigitalValue",       &ABFFH.nAlternateDigitalValue,
                                    PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(short)},

   {"nAlternateDigitalTrainValue",  &ABFFH.nAlternateDigitalTrainValue,
                                    PARAM_SHORT, ABF_EPOCHCOUNT, sizeof(short)},

   {"nAlternateDigitalOutputState", &ABFFH.nAlternateDigitalOutputState,
                                    PARAM_SHORT, 1, sizeof(short)},

   // Group #21 - Post-processing actions ( (4*16) + 16 = 80)

   {"fPostProcessLowpassFilter",     &ABFFH.fPostProcessLowpassFilter,
                                     PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"nPostProcessLowpassFilterType", &ABFFH.nPostProcessLowpassFilterType,
                                     PARAM_FLOAT, ABF_ADCCOUNT, sizeof(float)},

   {"",                          NULL,
                                 0, 0, 0},
};

//==============================================================================================
// FUNCTION:   CP_SetShowParam
// PURPOSE:    
// PARAMETERS: Set Parameter and then Show it
//    pszFileName - file name
//    pszParam    - string containing parameter value
//    pszVal
//    CPIndex
//    bShowIndexAsChar
// RETURNS:
//    current argument string
//
int CP_SetShowParam(char *pszFileName, char *pszParam, char *pszVal, int CPIndex, int bShowIndexAsChar)
{   
   int i, nError;
   HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE)
      return CP_EOPENFILE;

   BOOL bOK = ABFH_ParamReader(hFile, &ABFFH, &nError);
   CloseHandle(hFile);

   if (!bOK)
      return(CP_EINVALIDFILE);

   for (i = 0; ABFTYPE(i); i++)
   {   
      if (stricmp(pszParam, ABFNAME(i)) == 0)  // found it
         break;
   }

   if (!ABFTYPE(i))
      return(CP_ENOTFOUND);

   if ((CPIndex >= ABFNUMARGS(i)) || (CPIndex < 0))
      return(CP_EBADINDEX);

   if (pszVal)
   {   
      SetParam(i, CPIndex, pszVal);

      hFile = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile == INVALID_HANDLE_VALUE)
         return CP_EOPENFILE;

      // Write the header out to the file.
      DWORD dwBytesWritten;
      bOK = WriteFile(hFile, &ABFFH, ABF_HEADERSIZE, &dwBytesWritten, NULL);
      CloseHandle(hFile);

      if (!bOK)
         return CP_EINVALIDFILE;
   }

   ShowParam(i, CPIndex, pszVal!=NULL, bShowIndexAsChar);

   return (CP_SUCCESS);
}

//==============================================================================================
// FUNCTION:   SetParam
// PURPOSE:   Set a parameter value
// PARAMETERS:
//    i        - parameter for ABFTYPE fn, to specify data type of parameter
//    CPIndex  - parameter index
//    pszVal   - string containing parameter value
//
static void SetParam(int i, int CPIndex, char *pszVal)
{   
   char szBuf[256];

   switch(ABFTYPE(i))
   {   
      case PARAM_STRING:
         memset(szBuf, ' ', ABFSIZE(i));
         szBuf[ABFSIZE(i)] = '\0';
         memcpy(szBuf, pszVal, strlen(pszVal));
         memcpy(ABFSVALUE(i), szBuf, ABFSIZE(i) );
         break;
      case PARAM_SHORT:
         ABFNVALUE(i) = (short)strtol(pszVal, &pszVal, 10);
         break;
      case PARAM_BYTE:
         ABFNVALUE(i) = (char)strtol(pszVal, &pszVal, 10);
         break;
      case PARAM_LONG:
         ABFLVALUE(i) = strtol(pszVal, &pszVal, 10);
         break;
      case PARAM_FLOAT:
         ABFFVALUE(i) = (float)strtod(pszVal, &pszVal);
         break;
      case PARAM_DOUBLE:
         ABFDVALUE(i) = (double)strtod(pszVal, &pszVal);
         break;
   }
}

//==============================================================================================
// FUNCTION:   ShowParam
// PURPOSE:    Display the value of the requested parameter
// PARAMETERS:
//    i                - parameter for ABFTYPE fn, to specify data type of parameter
//    CPIndex          - parameter index
//    bNewValue        - flag to identify parameter as a new value
//    bShowIndexAsChar - flag to indicate whether to display index as char or numeric value
//
static void ShowParam(int i, int CPIndex, int bNewValue, int bShowIndexAsChar)
{   
   char szBuf[256];
   char szIndex[10];
   char *pszNow;

   pszNow = bNewValue ? "now " : "";

   if (ABFNUMARGS(i) < 2)
      szIndex[0] = '\0';
   else if (bShowIndexAsChar)
      sprintf(szIndex, "(%c)", 'A' + CPIndex);
   else
      sprintf(szIndex, "(%d)", CPIndex);

   fprintf(stdout, "\n%s%s %scontains the ", ABFNAME(i), szIndex, pszNow);

   switch(ABFTYPE(i))
   {   
      case PARAM_STRING:
         memcpy(szBuf, ABFSVALUE(i), ABFSIZE(i));
         szBuf[ABFSIZE(i)] = '\0';
         fprintf(stdout, "string ");
         if (ABFSIZE(i) > 40)
            fprintf(stdout, "\n");
         fprintf(stdout, "`%s'.\n", szBuf);
         break;
      case PARAM_SHORT:
         fprintf(stdout, "2-byte short integer %d.\n", ABFNVALUE(i));
         break;
      case PARAM_LONG:
         fprintf(stdout, "4-byte long integer %ld.\n", ABFLVALUE(i));
         break;
      case PARAM_FLOAT:
         fprintf(stdout, "4-byte real %g.\n", ABFFVALUE(i));
         break;
      case PARAM_DOUBLE:
         fprintf(stdout, "8-byte real %g.\n", ABFFVALUE(i));
         break;
   }
}

//==============================================================================================
// FUNCTION:   CP_DumpHeaderOffsets
// PURPOSE:    
// PARAMETERS: Dump name and offset of all header parameters
//    pszFileName - file name
// RETURNS:
//    current argument string
//
int CP_DumpHeaderOffsets(char *pszFileName )
{   
   int i, nError;
   HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE)
      return CP_EOPENFILE;

   BOOL bOK = ABFH_ParamReader(hFile, &ABFFH, &nError);
   CloseHandle(hFile);

   if (!bOK)
      return(CP_EINVALIDFILE);

   hFile = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE)
      return CP_EOPENFILE;

   // dump all names, offsets and values
   for(i=0; i<sizeof(ABFFileHeader); i++)
   {
      int nOffset = (int)ABFADDRESS(i) - (int)ABFADDRESS(0);
      if(nOffset > sizeof(ABFFileHeader))
         break;
    
      fprintf(stdout, "\nbyte offset %d at %s", nOffset, ABFNAME(i));
   }

   CloseHandle(hFile);
   return (CP_SUCCESS);
}

//==============================================================================================
// FUNCTION:   CP_ShowError
// PURPOSE:    Display error message
// PARAMETERS:
//    ErrorNum - number of errors
//
void CP_ShowError(int ErrorNum)
{   
   char szBuf[80];

   sprintf(szBuf, "\nError: %s.\n", CP_Errors[ErrorNum]);
   fprintf(stdout, szBuf);
}
