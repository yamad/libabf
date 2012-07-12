#include "swap.h"
#include "abf2.h"

int abf2_needs_swap(const char *buf)
{
    uint32_t filesig = read_uint32(buf, 0, 0);
    if (ABF2_FILESIGNATURE == filesig) {
        return 0;
    }
    uint32_t abf2_revsig = _swap32(ABF2_FILESIGNATURE);
    if (abf2_revsig == filesig) {
        return 1;
    }
    return -1;
}

/* get byte offset (location) of data for an abf2 section */
size_t abf2_section_block_offset(const struct abf2_section *sec)
{
    return abf2_get_block_offset(sec->uBlockIndex);
}

/* get byte offset (location) of given block index */
size_t abf2_get_block_offset(uint32_t block)
{
    return (size_t) (block * ABF2_BLOCKSIZE);
}

char *abf2_read_guidp(char *buf, struct guid *guid, bool to_swap)
{
    int i;
    buf = read_uint32p(buf, &(guid->Data1), to_swap);
    buf = read_uint16p(buf, &(guid->Data2), to_swap);
    buf = read_uint16p(buf, &(guid->Data3), to_swap);
    for (i = 0; i < 8; i++) {
        buf = read_uint8p(buf, &(guid->Data4[i]));
    }
    return buf;                 /* total = 128 bytes */
}

char *abf2_read_sectionp(char *buf, struct abf2_section *sec, bool to_swap)
{
    buf = read_uint32p(buf, &(sec->uBlockIndex), to_swap);
    buf = read_uint32p(buf, &(sec->uBytes), to_swap);
    buf = read_int64p(buf, &(sec->llNumEntries), to_swap);
    return buf;                 /* total = 16 bytes */
}

char *abf2_read_fileinfo(char *buf, struct abf2_fileinfo *finfo, bool to_swap)
{
    /* deserialize structure members manually */
    buf = read_uint32p(buf, &(finfo->uFileSignature), to_swap);
    buf = read_uint32p(buf, &(finfo->uFileVersionNumber), to_swap);
    buf = read_uint32p(buf, &(finfo->uFileInfoSize), to_swap);
    buf = read_uint32p(buf, &(finfo->uActualEpisodes), to_swap);
    buf = read_uint32p(buf, &(finfo->uFileStartDate), to_swap);
    buf = read_uint32p(buf, &(finfo->uFileStartTimeMS), to_swap);
    buf = read_uint32p(buf, &(finfo->uStopwatchTime), to_swap);
    buf = read_int16p(buf, &(finfo->nFileType), to_swap);
    buf = read_int16p(buf, &(finfo->nDataFormat), to_swap);
    buf = read_int16p(buf, &(finfo->nSimultaneousScan), to_swap);
    buf = read_int16p(buf, &(finfo->nCRCEnable), to_swap);
    buf = read_uint32p(buf, &(finfo->uFileCRC), to_swap);
    buf = abf2_read_guidp(buf, &(finfo->FileGUID), to_swap);
    buf = read_uint32p(buf, &(finfo->uCreatorVersion), to_swap);
    buf = read_uint32p(buf, &(finfo->uCreatorNameIndex), to_swap);
    buf = read_uint32p(buf, &(finfo->uModifierVersion), to_swap);
    buf = read_uint32p(buf, &(finfo->uModifierNameIndex), to_swap);
    buf = read_uint32p(buf, &(finfo->uProtocolPathIndex), to_swap);

    buf = abf2_read_sectionp(buf, &(finfo->ProtocolSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->ADCSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->DACSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->EpochSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->ADCPerDACSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->EpochPerDACSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->UserListSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->StatsRegionSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->MathSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->StringsSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->DataSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->TagSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->ScopeSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->DeltaSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->VoiceTagSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->SynchArraySection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->AnnotationSection), to_swap);
    buf = abf2_read_sectionp(buf, &(finfo->StatsSection), to_swap);
    buf += 148;                 /* skip unused bytes */
    return buf;                 /* total = 512 bytes */
}

char *abf2_read_protocolinfo(char *buf, struct abf2_protocolinfo *pinfo, bool to_swap)
{
    int i;
    buf = read_int16p(buf, &(pinfo->nOperationMode), to_swap);
    buf = read_float32p(buf, &(pinfo->fADCSequenceInterval), to_swap);

    buf = read_uint8p(buf, (uint8_t*)&(pinfo->bEnableFileCompression));
    buf += 3;                   /* skip unused bytes */

    buf = read_uint32p(buf, &(pinfo->uFileCompressionRatio), to_swap);
    buf = read_float32p(buf, &(pinfo->fSynchTimeUnit), to_swap);
    buf = read_float32p(buf, &(pinfo->fSecondsPerRun), to_swap);
    buf = read_int32p(buf, &(pinfo->lNumSamplesPerEpisode), to_swap);
    buf = read_int32p(buf, &(pinfo->lPreTriggerSamples), to_swap);
    buf = read_int32p(buf, &(pinfo->lEpisodesPerRun), to_swap);
    buf = read_int32p(buf, &(pinfo->lRunsPerTrial), to_swap);
    buf = read_int32p(buf, &(pinfo->lNumberOfTrials), to_swap);
    buf = read_int16p(buf, &(pinfo->nAveragingMode), to_swap);
    buf = read_int16p(buf, &(pinfo->nUndoRunCount), to_swap);
    buf = read_int16p(buf, &(pinfo->nFirstEpisodeInRun), to_swap);
    buf = read_float32p(buf, &(pinfo->fTriggerThreshold), to_swap);
    buf = read_int16p(buf, &(pinfo->nTriggerSource), to_swap);
    buf = read_int16p(buf, &(pinfo->nTriggerAction), to_swap);
    buf = read_int16p(buf, &(pinfo->nTriggerPolarity), to_swap);
    buf = read_float32p(buf, &(pinfo->fScopeOutputInterval), to_swap);
    buf = read_float32p(buf, &(pinfo->fEpisodeStartToStart), to_swap);
    buf = read_float32p(buf, &(pinfo->fRunStartToStart), to_swap);
    buf = read_int32p(buf, &(pinfo-> lAverageCount), to_swap);
    buf = read_float32p(buf, &(pinfo->fTrialStartToStart), to_swap);
    buf = read_int16p(buf, &(pinfo->nAutoTriggerStrategy), to_swap);
    buf = read_float32p(buf, &(pinfo->fFirstRunDelayS), to_swap);
    buf = read_int16p(buf, &(pinfo->nChannelStatsStrategy), to_swap);
    buf = read_int32p(buf, &(pinfo->lSamplesPerTrace), to_swap);
    buf = read_int32p(buf, &(pinfo->lStartDisplayNum), to_swap);
    buf = read_int32p(buf, &(pinfo->lFinishDisplayNum), to_swap);
    buf = read_int16p(buf, &(pinfo->nShowPNRawData), to_swap);
    buf = read_float32p(buf, &(pinfo->fStatisticsPeriod), to_swap);
    buf = read_int32p(buf, &(pinfo->lStatisticsMeasurements), to_swap);
    buf = read_int16p(buf, &(pinfo->nStatisticsSaveStrategy), to_swap);
    buf = read_float32p(buf, &(pinfo->fADCRange), to_swap);
    buf = read_float32p(buf, &(pinfo->fDACRange), to_swap);
    buf = read_int32p(buf, &(pinfo->lADCResolution), to_swap);
    buf = read_int32p(buf, &(pinfo->lDACResolution), to_swap);
    buf = read_int16p(buf, &(pinfo->nExperimentType), to_swap);
    buf = read_int16p(buf, &(pinfo->nManualInfoStrategy), to_swap);
    buf = read_int16p(buf, &(pinfo->nCommentsEnable), to_swap);
    buf = read_int32p(buf, &(pinfo->lFileCommentIndex), to_swap);
    buf = read_int16p(buf, &(pinfo->nAutoAnalyseEnable), to_swap);
    buf = read_int16p(buf, &(pinfo->nSignalType), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitalEnable), to_swap);
    buf = read_int16p(buf, &(pinfo->nActiveDACChannel), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitalHolding), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitalInterEpisode), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitalDACChannel), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitalTrainActiveLogic), to_swap);
    buf = read_int16p(buf, &(pinfo->nStatsEnable), to_swap);
    buf = read_int16p(buf, &(pinfo->nStatisticsClearStrategy), to_swap);
    buf = read_int16p(buf, &(pinfo->nLevelHysteresis), to_swap);
    buf = read_int32p(buf, &(pinfo->lTimeHysteresis), to_swap);
    buf = read_int16p(buf, &(pinfo->nAllowExternalTags), to_swap);
    buf = read_int16p(buf, &(pinfo->nAverageAlgorithm), to_swap);
    buf = read_float32p(buf, &(pinfo->fAverageWeighting), to_swap);
    buf = read_int16p(buf, &(pinfo->nUndoPromptStrategy), to_swap);
    buf = read_int16p(buf, &(pinfo->nTrialTriggerSource), to_swap);
    buf = read_int16p(buf, &(pinfo->nStatisticsDisplayStrategy), to_swap);
    buf = read_int16p(buf, &(pinfo->nExternalTagType), to_swap);
    buf = read_int16p(buf, &(pinfo->nScopeTriggerOut), to_swap);
    buf = read_int16p(buf, &(pinfo->nLTPType), to_swap);
    buf = read_int16p(buf, &(pinfo->nAlternateDACOutputState), to_swap);
    buf = read_int16p(buf, &(pinfo->nAlternateDigitalOutputState), to_swap);

    for (i=0; i<3; i++) {
        buf = read_float32p(buf, &(pinfo->fCellID[i]), to_swap);
    }

    buf = read_int16p(buf, &(pinfo->nDigitizerADCs), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitizerDACs), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitizerTotalDigitalOuts), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitizerSynchDigitalOuts), to_swap);
    buf = read_int16p(buf, &(pinfo->nDigitizerType), to_swap);
    buf += 304;                 /* skip unused bytes */
    return buf;
}


int abf2_print_fileinfo(const struct abf2_fileinfo *finfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-20s: %lu\n", spaces, "uFileSignature", finfo->uFileSignature);
    printf("%s%-20s: %lu\n", spaces, "uFileVersionNumber", finfo->uFileVersionNumber);
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
        printf("%s%-28s: %f\n", spaces, "fCellID[]", pinfo->fCellID[i]);
    }

    printf("%s%-28s: %d\n", spaces, "nDigitizerADCs", pinfo->nDigitizerADCs);
    printf("%s%-28s: %d\n", spaces, "nDigitizerDACs", pinfo->nDigitizerDACs);
    printf("%s%-28s: %d\n", spaces, "nDigitizerTotalDigitalOuts", pinfo->nDigitizerTotalDigitalOuts);
    printf("%s%-28s: %d\n", spaces, "nDigitizerSynchDigitalOuts", pinfo->nDigitizerSynchDigitalOuts);
    printf("%s%-28s: %d\n", spaces, "nDigitizerType", pinfo->nDigitizerType);
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
