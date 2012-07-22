#include "swap.h"
#include "abf2.h"

/* check that `buf` starts with a valid ABF2 file signature */
int abf2_can_open(const uint8_t *buf)
{
    uint32_t filesig = read_uint32(buf, 0, 0);
    return abf2_verify_filesignature(filesig);
}

/* verify `filesig` is a valid ABF2 file signature */
int abf2_verify_filesignature(uint32_t filesig)
{
    if (ABF2_FILESIGNATURE == filesig || \
        ABF2_REVFILESIGNATURE == filesig) {
        return 1;
    }
    return 0;
}

/* return true if the file endian is different than the host endian*/
int abf2_needs_swap(const uint8_t *buf)
{
    uint32_t filesig = read_uint32(buf, 0, 0);
    if (ABF2_FILESIGNATURE == filesig) {
        return 0;
    }
    if (ABF2_REVFILESIGNATURE == filesig) {
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

uint8_t *abf2_read_guidp(uint8_t *buf,
                         struct guid *guid,
                         bool to_swap)
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

uint8_t *abf2_read_sectionp(uint8_t *buf,
                            struct abf2_section *sec,
                            bool to_swap)
{
    buf = read_uint32p(buf, &(sec->uBlockIndex), to_swap);
    buf = read_uint32p(buf, &(sec->uBytes), to_swap);
    buf = read_int64p(buf, &(sec->llNumEntries), to_swap);
    return buf;                 /* total = 16 bytes */
}

uint8_t *abf2_read_fileinfo(uint8_t *buf, struct abf2_fileinfo *finfo, bool to_swap)
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

uint8_t *abf2_read_protocolinfo(uint8_t *buf,
                                struct abf2_protocolinfo *pinfo,
                                bool to_swap)
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
    return buf;                 /* total = 512 bytes */
}

uint8_t *abf2_read_mathinfo(uint8_t *buf,
                            struct abf2_mathinfo *minfo,
                            bool to_swap)
{
    int i;
    buf = read_int16p(buf, &(minfo->nMathEnable), to_swap);
    buf = read_int16p(buf, &(minfo->nMathExpression), to_swap);
    buf = read_uint32p(buf, &(minfo->uMathOperatorIndex), to_swap);
    buf = read_uint32p(buf, &(minfo->uMathUnitsIndex), to_swap);
    buf = read_float32p(buf, &(minfo->fMathUpperLimit), to_swap);
    buf = read_float32p(buf, &(minfo->fMathLowerLimit), to_swap);
    for (i=0; i<2; i++)
        buf = read_int16p(buf, &(minfo->nMathADCNum[i]), to_swap);
    for (i=0; i<16; i++)
        buf = read_int8p(buf, &(minfo->sUnused[i]));
    for (i=0; i<6; i++)
        buf = read_float32p(buf, &(minfo->fMathK[i]), to_swap);
    buf += 64;                   /* skip unused bytes */
    return buf;                  /* total = 128 bytes */
};

uint8_t *abf2_read_adcinfo(uint8_t *buf,
                           struct abf2_adcinfo *ainfo,
                           bool to_swap)
{
    buf = read_int16p(buf, &(ainfo->nADCNum), to_swap);
    buf = read_int16p(buf, &(ainfo->nTelegraphEnable), to_swap);
    buf = read_int16p(buf, &(ainfo->nTelegraphInstrument), to_swap);
    buf = read_float32p(buf, &(ainfo->fTelegraphAdditGain), to_swap);
    buf = read_float32p(buf, &(ainfo->fTelegraphFilter), to_swap);
    buf = read_float32p(buf, &(ainfo->fTelegraphMembraneCap), to_swap);
    buf = read_int16p(buf, &(ainfo->nTelegraphMode), to_swap);
    buf = read_float32p(buf, &(ainfo->fTelegraphAccessResistance), to_swap);
    buf = read_int16p(buf, &(ainfo->nADCPtoLChannelMap), to_swap);
    buf = read_int16p(buf, &(ainfo->nADCSamplingSeq), to_swap);
    buf = read_float32p(buf, &(ainfo->fADCProgrammableGain), to_swap);
    buf = read_float32p(buf, &(ainfo->fADCDisplayAmplification), to_swap);
    buf = read_float32p(buf, &(ainfo->fADCDisplayOffset), to_swap);
    buf = read_float32p(buf, &(ainfo->fInstrumentScaleFactor), to_swap);
    buf = read_float32p(buf, &(ainfo->fInstrumentOffset), to_swap);
    buf = read_float32p(buf, &(ainfo->fSignalGain), to_swap);
    buf = read_float32p(buf, &(ainfo->fSignalOffset), to_swap);
    buf = read_float32p(buf, &(ainfo->fSignalLowpassFilter), to_swap);
    buf = read_float32p(buf, &(ainfo->fSignalHighpassFilter), to_swap);
    buf = read_int8p(buf, &(ainfo->nLowpassFilterType));
    buf = read_int8p(buf, &(ainfo->nHighpassFilterType));
    buf = read_float32p(buf, &(ainfo->fPostProcessLowpassFilter), to_swap);
    buf = read_int8p(buf, &(ainfo->nPostProcessLowpassFilterType));
    buf = read_uint8p(buf, (uint8_t*)&(ainfo->bEnabledDuringPN));
    buf = read_int16p(buf, &(ainfo->nStatsChannelPolarity), to_swap);
    buf = read_int32p(buf, &(ainfo->lADCChannelNameIndex), to_swap);
    buf = read_int32p(buf, &(ainfo->lADCUnitsIndex), to_swap);
    buf += 46;                      /* skip unused bytes */
    return buf;                     /* total = 128 bytes */
};

uint8_t *abf2_read_dacinfo(uint8_t *buf,
                           struct abf2_dacinfo *dinfo,
                           bool to_swap)
{
    buf = read_int16p(buf, &(dinfo->nDACNum), to_swap);
    buf = read_int16p(buf, &(dinfo->nTelegraphDACScaleFactorEnable), to_swap);
    buf = read_float32p(buf, &(dinfo->fInstrumentHoldingLevel), to_swap);
    buf = read_float32p(buf, &(dinfo->fDACScaleFactor), to_swap);
    buf = read_float32p(buf, &(dinfo->fDACHoldingLevel), to_swap);
    buf = read_float32p(buf, &(dinfo->fDACCalibrationFactor), to_swap);
    buf = read_float32p(buf, &(dinfo->fDACCalibrationOffset), to_swap);
    buf = read_int32p(buf, &(dinfo->lDACChannelNameIndex), to_swap);
    buf = read_int32p(buf, &(dinfo->lDACChannelUnitsIndex), to_swap);
    buf = read_int32p(buf, &(dinfo->lDACFilePtr), to_swap);
    buf = read_int32p(buf, &(dinfo->lDACFileNumEpisodes), to_swap);
    buf = read_int16p(buf, &(dinfo->nWaveformEnable), to_swap);
    buf = read_int16p(buf, &(dinfo->nWaveformSource), to_swap);
    buf = read_int16p(buf, &(dinfo->nInterEpisodeLevel), to_swap);
    buf = read_float32p(buf, &(dinfo->fDACFileScale), to_swap);
    buf = read_float32p(buf, &(dinfo->fDACFileOffset), to_swap);
    buf = read_int32p(buf, &(dinfo->lDACFileEpisodeNum), to_swap);
    buf = read_int16p(buf, &(dinfo->nDACFileADCNum), to_swap);
    buf = read_int16p(buf, &(dinfo->nConditEnable), to_swap);
    buf = read_int32p(buf, &(dinfo->lConditNumPulses), to_swap);
    buf = read_float32p(buf, &(dinfo->fBaselineDuration), to_swap);
    buf = read_float32p(buf, &(dinfo->fBaselineLevel), to_swap);
    buf = read_float32p(buf, &(dinfo->fStepDuration), to_swap);
    buf = read_float32p(buf, &(dinfo->fStepLevel), to_swap);
    buf = read_float32p(buf, &(dinfo->fPostTrainPeriod), to_swap);
    buf = read_float32p(buf, &(dinfo->fPostTrainLevel), to_swap);
    buf = read_int16p(buf, &(dinfo->nMembTestEnable), to_swap);
    buf = read_int16p(buf, &(dinfo->nLeakSubtractType), to_swap);
    buf = read_int16p(buf, &(dinfo->nPNPolarity), to_swap);
    buf = read_float32p(buf, &(dinfo->fPNHoldingLevel), to_swap);
    buf = read_int16p(buf, &(dinfo->nPNNumADCChannels), to_swap);
    buf = read_int16p(buf, &(dinfo->nPNPosition), to_swap);
    buf = read_int16p(buf, &(dinfo->nPNNumPulses), to_swap);
    buf = read_float32p(buf, &(dinfo->fPNSettlingTime), to_swap);
    buf = read_float32p(buf, &(dinfo->fPNInterpulse), to_swap);
    buf = read_int16p(buf, &(dinfo->nLTPUsageOfDAC), to_swap);
    buf = read_int16p(buf, &(dinfo->nLTPPresynapticPulses), to_swap);
    buf = read_int32p(buf, &(dinfo->lDACFilePathIndex), to_swap);
    buf = read_float32p(buf, &(dinfo->fMembTestPreSettlingTimeMS), to_swap);
    buf = read_float32p(buf, &(dinfo->fMembTestPostSettlingTimeMS), to_swap);
    buf = read_int16p(buf, &(dinfo->nLeakSubtractADCIndex), to_swap);
    buf += 124;                 /* skip unused bytes */
    return buf;                 /* total = 256 bytes */
};

uint8_t *abf2_read_epochinfoperdac(uint8_t *buf,
                                   struct abf2_epochinfoperdac *einfo,
                                   bool to_swap)
{
    buf = read_int16p(buf, &(einfo->nEpochNum), to_swap);
    buf = read_int16p(buf, &(einfo->nDACNum), to_swap);
    buf = read_int16p(buf, &(einfo->nEpochType), to_swap);
    buf = read_float32p(buf, &(einfo->fEpochInitLevel), to_swap);
    buf = read_float32p(buf, &(einfo->fEpochLevelInc), to_swap);
    buf = read_int32p(buf, &(einfo->lEpochInitDuration), to_swap);
    buf = read_int32p(buf, &(einfo->lEpochDurationInc), to_swap);
    buf = read_int32p(buf, &(einfo->lEpochPulsePeriod), to_swap);
    buf = read_int32p(buf, &(einfo->lEpochPulseWidth), to_swap);
    buf += 18;                  /* skip unused bytes */
    return buf;                 /* total = 48 bytes */
};

uint8_t *abf2_read_epochinfo(uint8_t *buf,
                             struct abf2_epochinfo *einfo,
                             bool to_swap)
{
    buf = read_int16p(buf, &(einfo->nEpochNum), to_swap);
    buf = read_int16p(buf, &(einfo->nDigitalValue), to_swap);
    buf = read_int16p(buf, &(einfo->nDigitalTrainValue), to_swap);
    buf = read_int16p(buf, &(einfo->nAlternateDigitalValue), to_swap);
    buf = read_int16p(buf, &(einfo->nAlternateDigitalTrainValue), to_swap);
    buf = read_uint8p(buf, (uint8_t*)&(einfo->bEpochCompression));
    buf += 21;                  /* skip unused bytes */
    return buf;                 /* total = 32 bytes */
}

uint8_t *abf2_read_statsregioninfo(uint8_t *buf,
                                   struct abf2_statsregioninfo *sinfo,
                                   bool to_swap)
{
    buf = read_int16p(buf, &(sinfo->nRegionNum), to_swap);
    buf = read_int16p(buf, &(sinfo->nADCNum), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsActiveChannels), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsSearchRegionFlags), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsSelectedRegion), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsSmoothing), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsSmoothingEnable), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsBaseline), to_swap);
    buf = read_int32p(buf, &(sinfo->lStatsBaselineStart), to_swap);
    buf = read_int32p(buf, &(sinfo->lStatsBaselineEnd), to_swap);
    buf = read_int32p(buf, &(sinfo->lStatsMeasurements), to_swap);
    buf = read_int32p(buf, &(sinfo->lStatsStart), to_swap);
    buf = read_int32p(buf, &(sinfo->lStatsEnd), to_swap);
    buf = read_int16p(buf, &(sinfo->nRiseBottomPercentile), to_swap);
    buf = read_int16p(buf, &(sinfo->nRiseTopPercentile), to_swap);
    buf = read_int16p(buf, &(sinfo->nDecayBottomPercentile), to_swap);
    buf = read_int16p(buf, &(sinfo->nDecayTopPercentile), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsSearchMode), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsSearchDAC), to_swap);
    buf = read_int16p(buf, &(sinfo->nStatsBaselineDAC), to_swap);
    buf += 78;                  /* skip unused bytes */
    return buf;                 /* total = 128 bytes */
};

uint8_t *abf2_read_userlistinfo(uint8_t *buf,
                                struct abf2_userlistinfo *uinfo,
                                bool to_swap)
{
    buf = read_int16p(buf, &(uinfo->nListNum), to_swap);
    buf = read_int16p(buf, &(uinfo->nULEnable), to_swap);
    buf = read_int16p(buf, &(uinfo->nULParamToVary), to_swap);
    buf = read_int16p(buf, &(uinfo->nULRepeat), to_swap);
    buf = read_int32p(buf, &(uinfo->lULParamValueListIndex), to_swap);
    buf += 52;                                          /* skip unused bytes */
    return buf;                                         /* total = 64 bytes */
};
