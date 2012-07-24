#include "abf2_print.h"

int abf2_print_guid(const struct guid *guid, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s", spaces);
    printf("%X-", guid->Data1);
    printf("%X-", guid->Data2);
    printf("%X-", guid->Data3);
    int i;
    for (i=0; i<2; i++)
        printf("%X", guid->Data4[i]);
    printf("-");
    for (;i<8; i++)
        printf("%X", guid->Data4[i]);
    printf("\n");
    free(spaces);
    return 0;
}

int abf2_print_section(const struct abf2_section *sec, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-12s: %lu\n", spaces, "uBlockIndex", sec->uBlockIndex);
    printf("%s%-12s: %lu\n", spaces, "uBytes", sec->uBytes);
    printf("%s%-12s: %llu\n", spaces, "llNumEntries", sec->llNumEntries);
    free(spaces);
    return 0;
}

int abf2_print_fileinfo(const struct abf2_fileinfo *finfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-20s: %#x\n", spaces, "uFileSignature", finfo->uFileSignature);
    printf("%s%-20s: %#x\n", spaces, "uFileVersionNumber", finfo->uFileVersionNumber);
    printf("%s%-20s: %lu\n", spaces, "uFileInfoSize", finfo->uFileInfoSize);
    printf("%s%-20s: %lu\n", spaces, "uActualEpisodes", finfo->uActualEpisodes);
    printf("%s%-20s: %lu\n", spaces, "uFileStartDate", finfo->uFileStartDate);
    printf("%s%-20s: %lu\n", spaces, "uFileStartTimeMS", finfo->uFileStartTimeMS);
    printf("%s%-20s: %lu\n", spaces, "uStopwatchTime", finfo->uStopwatchTime);
    printf("%s%-20s: %d\n", spaces, "nFileType", finfo->nFileType);
    printf("%s%-20s: %d\n", spaces, "nDataFormat", finfo->nDataFormat);
    printf("%s%-20s: %d\n", spaces, "nSimultaneousScan", finfo->nSimultaneousScan);
    printf("%s%-20s: %d\n", spaces, "nCRCEnable", finfo->nCRCEnable);
    printf("%s%-20s: %lu\n", spaces, "uFileCRC", finfo->uFileCRC);
    printf("%s%-20s: ", spaces, "FileGUID");
    abf2_print_guid(&(finfo->FileGUID), 0);
    printf("%s%-20s: %lu\n", spaces, "uCreatorVersion", finfo->uCreatorVersion);
    printf("%s%-20s: %lu\n", spaces, "uCreatorNameIndex", finfo->uCreatorNameIndex);
    printf("%s%-20s: %lu\n", spaces, "uModifierVersion", finfo->uModifierVersion);
    printf("%s%-20s: %lu\n", spaces, "uModifierNameIndex", finfo->uModifierNameIndex);
    printf("%s%-20s: %lu\n", spaces, "uProtocolPathIndex", finfo->uProtocolPathIndex);

    printf("%s%-20s:\n", spaces, "ProtocolSection");
    abf2_print_section(&(finfo->ProtocolSection), 4);
    printf("%s%-20s:\n", spaces, "ADCSection");
    abf2_print_section(&(finfo->ADCSection), 4);
    printf("%s%-20s:\n", spaces, "DACSection");
    abf2_print_section(&(finfo->DACSection), 4);
    printf("%s%-20s:\n", spaces, "EpochSection");
    abf2_print_section(&(finfo->EpochSection), 4);
    printf("%s%-20s:\n", spaces, "ADCPerDACSection");
    abf2_print_section(&(finfo->ADCPerDACSection), 4);
    printf("%s%-20s:\n", spaces, "EpochPerDACSection");
    abf2_print_section(&(finfo->EpochPerDACSection), 4);
    printf("%s%-20s:\n", spaces, "UserListSection");
    abf2_print_section(&(finfo->UserListSection), 4);
    printf("%s%-20s:\n", spaces, "StatsRegionSection");
    abf2_print_section(&(finfo->StatsRegionSection), 4);
    printf("%s%-20s:\n", spaces, "MathSection");
    abf2_print_section(&(finfo->MathSection), 4);
    printf("%s%-20s:\n", spaces, "StringsSection");
    abf2_print_section(&(finfo->StringsSection), 4);
    printf("%s%-20s:\n", spaces, "DataSection");
    abf2_print_section(&(finfo->DataSection), 4);
    printf("%s%-20s:\n", spaces, "TagSection");
    abf2_print_section(&(finfo->TagSection), 4);
    printf("%s%-20s:\n", spaces, "ScopeSection");
    abf2_print_section(&(finfo->ScopeSection), 4);
    printf("%s%-20s:\n", spaces, "DeltaSection");
    abf2_print_section(&(finfo->DeltaSection), 4);
    printf("%s%-20s:\n", spaces, "VoiceTagSection");
    abf2_print_section(&(finfo->VoiceTagSection), 4);
    printf("%s%-20s:\n", spaces, "SynchArraySection");
    abf2_print_section(&(finfo->SynchArraySection), 4);
    printf("%s%-20s:\n", spaces, "AnnotationSection");
    abf2_print_section(&(finfo->AnnotationSection), 4);
    printf("%s%-20s:\n", spaces, "StatsSection");
    abf2_print_section(&(finfo->StatsSection), 4);
    free(spaces);
    return 0;
}

int abf2_print_protocolinfo(const struct abf2_protocolinfo *pinfo, int indent)
{
    int i;
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-28s: %d\n", spaces, "nOperationMode", pinfo->nOperationMode);
    printf("%s%-28s: %f\n", spaces, "fADCSequenceInterval", pinfo->fADCSequenceInterval);
    printf("%s%-28s: %d\n", spaces, "bEnableFileCompression", pinfo->bEnableFileCompression);
    printf("%s%-28s: %d\n", spaces, "uFileCompressionRatio", pinfo->uFileCompressionRatio);
    printf("%s%-28s: %f\n", spaces, "fSynchTimeUnit", pinfo->fSynchTimeUnit);
    printf("%s%-28s: %f\n", spaces, "fSecondsPerRun", pinfo->fSecondsPerRun);
    printf("%s%-28s: %d\n", spaces, "lNumSamplesPerEpisode", pinfo->lNumSamplesPerEpisode);
    printf("%s%-28s: %d\n", spaces, "lPreTriggerSamples", pinfo->lPreTriggerSamples);
    printf("%s%-28s: %d\n", spaces, "lEpisodesPerRun", pinfo->lEpisodesPerRun);
    printf("%s%-28s: %d\n", spaces, "lRunsPerTrial", pinfo->lRunsPerTrial);
    printf("%s%-28s: %d\n", spaces, "lNumberOfTrials", pinfo->lNumberOfTrials);
    printf("%s%-28s: %d\n", spaces, "nAveragingMode", pinfo->nAveragingMode);
    printf("%s%-28s: %d\n", spaces, "nUndoRunCount", pinfo->nUndoRunCount);
    printf("%s%-28s: %d\n", spaces, "nFirstEpisodeInRun", pinfo->nFirstEpisodeInRun);
    printf("%s%-28s: %f\n", spaces, "fTriggerThreshold", pinfo->fTriggerThreshold);
    printf("%s%-28s: %d\n", spaces, "nTriggerSource", pinfo->nTriggerSource);
    printf("%s%-28s: %d\n", spaces, "nTriggerAction", pinfo->nTriggerAction);
    printf("%s%-28s: %d\n", spaces, "nTriggerPolarity", pinfo->nTriggerPolarity);
    printf("%s%-28s: %f\n", spaces, "fScopeOutputInterval", pinfo->fScopeOutputInterval);
    printf("%s%-28s: %f\n", spaces, "fEpisodeStartToStart", pinfo->fEpisodeStartToStart);
    printf("%s%-28s: %f\n", spaces, "fRunStartToStart", pinfo->fRunStartToStart);
    printf("%s%-28s: %d\n", spaces, "lAverageCount", pinfo-> lAverageCount);
    printf("%s%-28s: %f\n", spaces, "fTrialStartToStart", pinfo->fTrialStartToStart);
    printf("%s%-28s: %d\n", spaces, "nAutoTriggerStrategy", pinfo->nAutoTriggerStrategy);
    printf("%s%-28s: %f\n", spaces, "fFirstRunDelayS", pinfo->fFirstRunDelayS);

    printf("%s%-28s: %d\n", spaces, "nChannelStatsStrategy", pinfo->nChannelStatsStrategy);
    printf("%s%-28s: %d\n", spaces, "lSamplesPerTrace", pinfo->lSamplesPerTrace);
    printf("%s%-28s: %d\n", spaces, "lStartDisplayNum", pinfo->lStartDisplayNum);
    printf("%s%-28s: %d\n", spaces, "lFinishDisplayNum", pinfo->lFinishDisplayNum);
    printf("%s%-28s: %d\n", spaces, "nShowPNRawData", pinfo->nShowPNRawData);
    printf("%s%-28s: %f\n", spaces, "fStatisticsPeriod", pinfo->fStatisticsPeriod);
    printf("%s%-28s: %d\n", spaces, "lStatisticsMeasurements", pinfo->lStatisticsMeasurements);
    printf("%s%-28s: %d\n", spaces, "nStatisticsSaveStrategy", pinfo->nStatisticsSaveStrategy);

    printf("%s%-28s: %f\n", spaces, "fADCRange", pinfo->fADCRange);
    printf("%s%-28s: %f\n", spaces, "fDACRange", pinfo->fDACRange);
    printf("%s%-28s: %d\n", spaces, "lADCResolution", pinfo->lADCResolution);
    printf("%s%-28s: %d\n", spaces, "lDACResolution", pinfo->lDACResolution);

    printf("%s%-28s: %d\n", spaces, "nExperimentType", pinfo->nExperimentType);
    printf("%s%-28s: %d\n", spaces, "nManualInfoStrategy", pinfo->nManualInfoStrategy);
    printf("%s%-28s: %d\n", spaces, "nCommentsEnable", pinfo->nCommentsEnable);
    printf("%s%-28s: %d\n", spaces, "lFileCommentIndex", pinfo->lFileCommentIndex);
    printf("%s%-28s: %d\n", spaces, "nAutoAnalyseEnable", pinfo->nAutoAnalyseEnable);
    printf("%s%-28s: %d\n", spaces, "nSignalType", pinfo->nSignalType);

    printf("%s%-28s: %d\n", spaces, "nDigitalEnable", pinfo->nDigitalEnable);
    printf("%s%-28s: %d\n", spaces, "nActiveDACChannel", pinfo->nActiveDACChannel);
    printf("%s%-28s: %d\n", spaces, "nDigitalHolding", pinfo->nDigitalHolding);
    printf("%s%-28s: %d\n", spaces, "nDigitalInterEpisode", pinfo->nDigitalInterEpisode);
    printf("%s%-28s: %d\n", spaces, "nDigitalDACChannel", pinfo->nDigitalDACChannel);
    printf("%s%-28s: %d\n", spaces, "nDigitalTrainActiveLogic", pinfo->nDigitalTrainActiveLogic);

    printf("%s%-28s: %d\n", spaces, "nStatsEnable", pinfo->nStatsEnable);
    printf("%s%-28s: %d\n", spaces, "nStatisticsClearStrategy", pinfo->nStatisticsClearStrategy);

    printf("%s%-28s: %d\n", spaces, "nLevelHysteresis", pinfo->nLevelHysteresis);
    printf("%s%-28s: %d\n", spaces, "lTimeHysteresis", pinfo->lTimeHysteresis);
    printf("%s%-28s: %d\n", spaces, "nAllowExternalTags", pinfo->nAllowExternalTags);
    printf("%s%-28s: %d\n", spaces, "nAverageAlgorithm", pinfo->nAverageAlgorithm);
    printf("%s%-28s: %f\n", spaces, "fAverageWeighting", pinfo->fAverageWeighting);
    printf("%s%-28s: %d\n", spaces, "nUndoPromptStrategy", pinfo->nUndoPromptStrategy);
    printf("%s%-28s: %d\n", spaces, "nTrialTriggerSource", pinfo->nTrialTriggerSource);
    printf("%s%-28s: %d\n", spaces, "nStatisticsDisplayStrategy", pinfo->nStatisticsDisplayStrategy);
    printf("%s%-28s: %d\n", spaces, "nExternalTagType", pinfo->nExternalTagType);
    printf("%s%-28s: %d\n", spaces, "nScopeTriggerOut", pinfo->nScopeTriggerOut);
    printf("%s%-28s: %d\n", spaces, "nLTPType", pinfo->nLTPType);
    printf("%s%-28s: %d\n", spaces, "nAlternateDACOutputState", pinfo->nAlternateDACOutputState);
    printf("%s%-28s: %d\n", spaces, "nAlternateDigitalOutputState", pinfo->nAlternateDigitalOutputState);

    for (i=0; i<3; i++) {
        printf("%sfCellID[%d]%-18s: %f\n", spaces, i, " ", pinfo->fCellID[i]);
    }

    printf("%s%-28s: %d\n", spaces, "nDigitizerADCs", pinfo->nDigitizerADCs);
    printf("%s%-28s: %d\n", spaces, "nDigitizerDACs", pinfo->nDigitizerDACs);
    printf("%s%-28s: %d\n", spaces, "nDigitizerTotalDigitalOuts", pinfo->nDigitizerTotalDigitalOuts);
    printf("%s%-28s: %d\n", spaces, "nDigitizerSynchDigitalOuts", pinfo->nDigitizerSynchDigitalOuts);
    printf("%s%-28s: %d\n", spaces, "nDigitizerType", pinfo->nDigitizerType);
    free(spaces);
    return 0;
}

int abf2_print_mathinfo(const struct abf2_mathinfo *minfo, int indent)
{
    int i;
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-20s: %d\n", spaces, "nMathEnable", minfo->nMathEnable);
    printf("%s%-20s: %d\n", spaces, "nMathExpression", minfo->nMathExpression);
    printf("%s%-20s: %ud\n", spaces, "uMathOperatorIndex", minfo->uMathOperatorIndex);
    printf("%s%-20s: %ud\n", spaces, "uMathUnitsIndex", minfo->uMathUnitsIndex);
    printf("%s%-20s: %f\n", spaces, "fMathUpperLimit", minfo->fMathUpperLimit);
    printf("%s%-20s: %f\n", spaces, "fMathLowerLimit", minfo->fMathLowerLimit);
    for (i=0; i<2; i++)
        printf("%snMathADCNum[%d]%-6s: %d\n", spaces, i, " ", minfo->nMathADCNum[i]);
    for (i=0; i<6; i++)
        printf("%sfMathK[%d]%-11s: %f\n", spaces, i, " ", minfo->fMathK[i]);
    free(spaces);
    return 0;
};

int abf2_print_adcinfo(const struct abf2_adcinfo *ainfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-29s: %d\n", spaces, "nADCNum", ainfo->nADCNum);
    printf("%s%-29s: %d\n", spaces, "nTelegraphEnable", ainfo->nTelegraphEnable);
    printf("%s%-29s: %d\n", spaces, "nTelegraphInstrument", ainfo->nTelegraphInstrument);
    printf("%s%-29s: %f\n", spaces, "fTelegraphAdditGain", ainfo->fTelegraphAdditGain);
    printf("%s%-29s: %f\n", spaces, "fTelegraphFilter", ainfo->fTelegraphFilter);
    printf("%s%-29s: %f\n", spaces, "fTelegraphMembraneCap", ainfo->fTelegraphMembraneCap);
    printf("%s%-29s: %d\n", spaces, "nTelegraphMode", ainfo->nTelegraphMode);
    printf("%s%-29s: %f\n", spaces, "fTelegraphAccessResistance", ainfo->fTelegraphAccessResistance);
    printf("%s%-29s: %d\n", spaces, "nADCPtoLChannelMap", ainfo->nADCPtoLChannelMap);
    printf("%s%-29s: %d\n", spaces, "nADCSamplingSeq", ainfo->nADCSamplingSeq);
    printf("%s%-29s: %f\n", spaces, "fADCProgrammableGain", ainfo->fADCProgrammableGain);
    printf("%s%-29s: %f\n", spaces, "fADCDisplayAmplification", ainfo->fADCDisplayAmplification);
    printf("%s%-29s: %f\n", spaces, "fADCDisplayOffset", ainfo->fADCDisplayOffset);
    printf("%s%-29s: %f\n", spaces, "fInstrumentScaleFactor", ainfo->fInstrumentScaleFactor);
    printf("%s%-29s: %f\n", spaces, "fInstrumentOffset", ainfo->fInstrumentOffset);
    printf("%s%-29s: %f\n", spaces, "fSignalGain", ainfo->fSignalGain);
    printf("%s%-29s: %f\n", spaces, "fSignalOffset", ainfo->fSignalOffset);
    printf("%s%-29s: %f\n", spaces, "fSignalLowpassFilter", ainfo->fSignalLowpassFilter);
    printf("%s%-29s: %f\n", spaces, "fSignalHighpassFilter", ainfo->fSignalHighpassFilter);
    printf("%s%-29s: %d\n", spaces, "nLowpassFilterType", ainfo->nLowpassFilterType);
    printf("%s%-29s: %d\n", spaces, "nHighpassFilterType", ainfo->nHighpassFilterType);
    printf("%s%-29s: %f\n", spaces, "fPostProcessLowpassFilter", ainfo->fPostProcessLowpassFilter);
    printf("%s%-29s: %d\n", spaces, "nPostProcessLowpassFilterType", ainfo->nPostProcessLowpassFilterType);
    printf("%s%-29s: %d\n", spaces, "bEnabledDuringPN", ainfo->bEnabledDuringPN);
    printf("%s%-29s: %d\n", spaces, "nStatsChannelPolarity", ainfo->nStatsChannelPolarity);
    printf("%s%-29s: %d\n", spaces, "lADCChannelNameIndex", ainfo->lADCChannelNameIndex);
    printf("%s%-29s: %d\n", spaces, "lADCUnitsIndex", ainfo->lADCUnitsIndex);
    free(spaces);
    return 0;
};

int abf2_print_dacinfo(const struct abf2_dacinfo *dinfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-30s: %d\n", spaces, "nDACNum", dinfo->nDACNum);
    printf("%s%-30s: %d\n", spaces, "nTelegraphDACScaleFactorEnable", dinfo->nTelegraphDACScaleFactorEnable);
    printf("%s%-30s: %f\n", spaces, "fInstrumentHoldingLevel", dinfo->fInstrumentHoldingLevel);
    printf("%s%-30s: %f\n", spaces, "fDACScaleFactor", dinfo->fDACScaleFactor);
    printf("%s%-30s: %f\n", spaces, "fDACHoldingLevel", dinfo->fDACHoldingLevel);
    printf("%s%-30s: %f\n", spaces, "fDACCalibrationFactor", dinfo->fDACCalibrationFactor);
    printf("%s%-30s: %f\n", spaces, "fDACCalibrationOffset", dinfo->fDACCalibrationOffset);
    printf("%s%-30s: %d\n", spaces, "lDACChannelNameIndex", dinfo->lDACChannelNameIndex);
    printf("%s%-30s: %d\n", spaces, "lDACChannelUnitsIndex", dinfo->lDACChannelUnitsIndex);
    printf("%s%-30s: %d\n", spaces, "lDACFilePtr", dinfo->lDACFilePtr);
    printf("%s%-30s: %d\n", spaces, "lDACFileNumEpisodes", dinfo->lDACFileNumEpisodes);
    printf("%s%-30s: %d\n", spaces, "nWaveformEnable", dinfo->nWaveformEnable);
    printf("%s%-30s: %d\n", spaces, "nWaveformSource", dinfo->nWaveformSource);
    printf("%s%-30s: %d\n", spaces, "nInterEpisodeLevel", dinfo->nInterEpisodeLevel);
    printf("%s%-30s: %f\n", spaces, "fDACFileScale", dinfo->fDACFileScale);
    printf("%s%-30s: %f\n", spaces, "fDACFileOffset", dinfo->fDACFileOffset);
    printf("%s%-30s: %d\n", spaces, "lDACFileEpisodeNum", dinfo->lDACFileEpisodeNum);
    printf("%s%-30s: %d\n", spaces, "nDACFileADCNum", dinfo->nDACFileADCNum);
    printf("%s%-30s: %d\n", spaces, "nConditEnable", dinfo->nConditEnable);
    printf("%s%-30s: %d\n", spaces, "lConditNumPulses", dinfo->lConditNumPulses);
    printf("%s%-30s: %f\n", spaces, "fBaselineDuration", dinfo->fBaselineDuration);
    printf("%s%-30s: %f\n", spaces, "fBaselineLevel", dinfo->fBaselineLevel);
    printf("%s%-30s: %f\n", spaces, "fStepDuration", dinfo->fStepDuration);
    printf("%s%-30s: %f\n", spaces, "fStepLevel", dinfo->fStepLevel);
    printf("%s%-30s: %f\n", spaces, "fPostTrainPeriod", dinfo->fPostTrainPeriod);
    printf("%s%-30s: %f\n", spaces, "fPostTrainLevel", dinfo->fPostTrainLevel);
    printf("%s%-30s: %d\n", spaces, "nMembTestEnable", dinfo->nMembTestEnable);
    printf("%s%-30s: %d\n", spaces, "nLeakSubtractType", dinfo->nLeakSubtractType);
    printf("%s%-30s: %d\n", spaces, "nPNPolarity", dinfo->nPNPolarity);
    printf("%s%-30s: %f\n", spaces, "fPNHoldingLevel", dinfo->fPNHoldingLevel);
    printf("%s%-30s: %d\n", spaces, "nPNNumADCChannels", dinfo->nPNNumADCChannels);
    printf("%s%-30s: %d\n", spaces, "nPNPosition", dinfo->nPNPosition);
    printf("%s%-30s: %d\n", spaces, "nPNNumPulses", dinfo->nPNNumPulses);
    printf("%s%-30s: %f\n", spaces, "fPNSettlingTime", dinfo->fPNSettlingTime);
    printf("%s%-30s: %f\n", spaces, "fPNInterpulse", dinfo->fPNInterpulse);
    printf("%s%-30s: %d\n", spaces, "nLTPUsageOfDAC", dinfo->nLTPUsageOfDAC);
    printf("%s%-30s: %d\n", spaces, "nLTPPresynapticPulses", dinfo->nLTPPresynapticPulses);
    printf("%s%-30s: %d\n", spaces, "lDACFilePathIndex", dinfo->lDACFilePathIndex);
    printf("%s%-30s: %f\n", spaces, "fMembTestPreSettlingTimeMS", dinfo->fMembTestPreSettlingTimeMS);
    printf("%s%-30s: %f\n", spaces, "fMembTestPostSettlingTimeMS", dinfo->fMembTestPostSettlingTimeMS);
    printf("%s%-30s: %d\n", spaces, "nLeakSubtractADCIndex", dinfo->nLeakSubtractADCIndex);
    free(spaces);
    return 0;
};

int abf2_print_epochinfoperdac(const struct abf2_epochinfoperdac *einfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-18s: %d\n", spaces, "nEpochNum", einfo->nEpochNum);
    printf("%s%-18s: %d\n", spaces, "nDACNum", einfo->nDACNum);
    printf("%s%-18s: %d\n", spaces, "nEpochType", einfo->nEpochType);
    printf("%s%-18s: %f\n", spaces, "fEpochInitLevel", einfo->fEpochInitLevel);
    printf("%s%-18s: %f\n", spaces, "fEpochLevelInc", einfo->fEpochLevelInc);
    printf("%s%-18s: %d\n", spaces, "lEpochInitDuration", einfo->lEpochInitDuration);
    printf("%s%-18s: %d\n", spaces, "lEpochDurationInc", einfo->lEpochDurationInc);
    printf("%s%-18s: %d\n", spaces, "lEpochPulsePeriod", einfo->lEpochPulsePeriod);
    printf("%s%-18s: %d\n", spaces, "lEpochPulseWidth", einfo->lEpochPulseWidth);
    free(spaces);
    return 0;
};

int abf2_print_epochinfo(const struct abf2_epochinfo *einfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-27s: %d\n", spaces, "nEpochNum", einfo->nEpochNum);
    printf("%s%-27s: %d\n", spaces, "nDigitalValue", einfo->nDigitalValue);
    printf("%s%-27s: %d\n", spaces, "nDigitalTrainValue", einfo->nDigitalTrainValue);
    printf("%s%-27s: %d\n", spaces, "nAlternateDigitalValue", einfo->nAlternateDigitalValue);
    printf("%s%-27s: %d\n", spaces, "nAlternateDigitalTrainValue", einfo->nAlternateDigitalTrainValue);
    printf("%s%-27s: %d\n", spaces, "bEpochCompression", einfo->bEpochCompression);
    free(spaces);
    return 0;
}

int abf2_print_statsregioninfo(const struct abf2_statsregioninfo *sinfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-23s: %d\n", spaces, "nRegionNum", sinfo->nRegionNum);
    printf("%s%-23s: %d\n", spaces, "nADCNum", sinfo->nADCNum);
    printf("%s%-23s: %d\n", spaces, "nStatsActiveChannels", sinfo->nStatsActiveChannels);
    printf("%s%-23s: %d\n", spaces, "nStatsSearchRegionFlags", sinfo->nStatsSearchRegionFlags);
    printf("%s%-23s: %d\n", spaces, "nStatsSelectedRegion", sinfo->nStatsSelectedRegion);
    printf("%s%-23s: %d\n", spaces, "nStatsSmoothing", sinfo->nStatsSmoothing);
    printf("%s%-23s: %d\n", spaces, "nStatsSmoothingEnable", sinfo->nStatsSmoothingEnable);
    printf("%s%-23s: %d\n", spaces, "nStatsBaseline", sinfo->nStatsBaseline);
    printf("%s%-23s: %d\n", spaces, "lStatsBaselineStart", sinfo->lStatsBaselineStart);
    printf("%s%-23s: %d\n", spaces, "lStatsBaselineEnd", sinfo->lStatsBaselineEnd);
    printf("%s%-23s: %d\n", spaces, "lStatsMeasurements", sinfo->lStatsMeasurements);
    printf("%s%-23s: %d\n", spaces, "lStatsStart", sinfo->lStatsStart);
    printf("%s%-23s: %d\n", spaces, "lStatsEnd", sinfo->lStatsEnd);
    printf("%s%-23s: %d\n", spaces, "nRiseBottomPercentile", sinfo->nRiseBottomPercentile);
    printf("%s%-23s: %d\n", spaces, "nRiseTopPercentile", sinfo->nRiseTopPercentile);
    printf("%s%-23s: %d\n", spaces, "nDecayBottomPercentile", sinfo->nDecayBottomPercentile);
    printf("%s%-23s: %d\n", spaces, "nDecayTopPercentile", sinfo->nDecayTopPercentile);
    printf("%s%-23s: %d\n", spaces, "nStatsSearchMode", sinfo->nStatsSearchMode);
    printf("%s%-23s: %d\n", spaces, "nStatsSearchDAC", sinfo->nStatsSearchDAC);
    printf("%s%-23s: %d\n", spaces, "nStatsBaselineDAC", sinfo->nStatsBaselineDAC);
    free(spaces);
    return 0;
};

int abf2_print_userlistinfo(const struct abf2_userlistinfo *uinfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-22s: %d\n", spaces, "nListNum", uinfo->nListNum);
    printf("%s%-22s: %d\n", spaces, "nULEnable", uinfo->nULEnable);
    printf("%s%-22s: %d\n", spaces, "nULParamToVary", uinfo->nULParamToVary);
    printf("%s%-22s: %d\n", spaces, "nULRepeat", uinfo->nULRepeat);
    printf("%s%-22s: %d\n", spaces, "lULParamValueListIndex", uinfo->lULParamValueListIndex);
    free(spaces);
    return 0;
};
