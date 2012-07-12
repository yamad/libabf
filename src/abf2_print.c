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

char *get_repeated_string(char c, int repeats) {
    char *dest = malloc(repeats + 1);
    if (dest == NULL) {
        printf("Memory failure.");
        exit(1);
    }
    if (repeats > 0) {
        memset(dest, c, repeats);
        *(dest + repeats) = '\0';
    }
    return dest;
}
