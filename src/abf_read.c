#include "abf_read.h"

char *abf_read_logfont(char *buf, struct abf_logfont *linfo, bool to_swap)
{
    buf = read_int16p(buf, &(linfo->nHeight), to_swap);
    buf = read_int16p(buf, &(linfo->nWeight), to_swap);
    buf = read_uint8p(buf, &(linfo->cPitchAndFamily));
    buf += 3;                   /* skip unused bytes */
    buf = read_charnp(buf, (char*)&(linfo->szFaceName), ABF_FACESIZE);
    return buf;                 /* total = 40 bytes */
}

char *abf_read_signal(char *buf, struct abf_signal *sinfo, bool to_swap)
{
    buf = read_charnp(buf, (char*)&(sinfo->szName), ABF_ADCNAMELEN+2);
    buf = read_int16p(buf, &(sinfo->nMxOffset), to_swap);
    buf = read_uint32p(buf, &(sinfo->rgbColor), to_swap);
    buf = read_uint8p(buf, &(sinfo->nPenWidth));
    buf = read_uint8p(buf, &(sinfo->bDrawPoints));
    buf = read_uint8p(buf, &(sinfo->bHidden));
    buf = read_uint8p(buf, &(sinfo->bFloatData));
    buf = read_float32p(buf, &(sinfo->fVertProportion), to_swap);
    buf = read_float32p(buf, &(sinfo->fDisplayGain), to_swap);
    buf = read_float32p(buf, &(sinfo->fDisplayOffset), to_swap);
    return buf;                 /* total = 34 bytes */
}

char *abf_read_scopeconfig(char *buf, struct abf_scopeconfig *sinfo, bool to_swap)
{
    int i;
    buf = read_uint32p(buf, &(sinfo->dwFlags), to_swap);
    for (i=0; i<ABF_SCOPECOLORS; i++) {
        buf = read_uint32p(buf, &(sinfo->rgbColor[i]), to_swap);
    }
    buf = read_float32p(buf, &(sinfo->fDisplayStart), to_swap);
    buf = read_float32p(buf, &(sinfo->fDisplayEnd), to_swap);
    buf = read_uint16p(buf, &(sinfo->wScopeMode), to_swap);
    buf = read_uint8p(buf, &(sinfo->bMaximized));
    buf = read_uint8p(buf, &(sinfo->bMinimized));
    buf = read_int16p(buf, &(sinfo->xLeft), to_swap);
    buf = read_int16p(buf, &(sinfo->yTop), to_swap);
    buf = read_int16p(buf, &(sinfo->xRight), to_swap);
    buf = read_int16p(buf, &(sinfo->yBottom), to_swap);

    buf = abf_read_logfont(buf, &(sinfo->LogFont), to_swap);
    for (i=0; i<ABF_ADCCOUNT; i++) {
        buf = abf_read_signal(buf, &(sinfo->TraceList[i]), to_swap);
    }
    buf = read_int16p(buf, &(sinfo->nYAxisWidth), to_swap);
    buf = read_int16p(buf, &(sinfo->nTraceCount), to_swap);
    buf = read_int16p(buf, &(sinfo->nEraseStrategy), to_swap);
    buf = read_int16p(buf, &(sinfo->nDockState), to_swap);

    buf = read_int16p(buf, &(sinfo->nSizeofOldStructure), to_swap);
    for (i=0; i<ABF_SCOPECOLORS_EX; i++) {
        buf = read_uint32p(buf, &(sinfo->rgbColorEx[i]), to_swap);
    }
    buf = read_int16p(buf, &(sinfo->nAutoZeroState), to_swap);
    buf = read_uint32p(buf, &(sinfo->dwCursorsVisibleState), to_swap);
    buf = read_uint32p(buf, &(sinfo->dwCursorsLockedState), to_swap);
    buf += 61;
    return buf;                 /* total = 769 bytes */
}

char *abf_read_tag(char *buf, struct abf_tag *tinfo, bool to_swap)
{
    buf = read_int32p(buf, &(tinfo->lTagTime), to_swap);
    buf = read_charnp(buf, (char*)&(tinfo->sComment), ABF_TAGCOMMENTLEN);
    buf = read_int16p(buf, &(tinfo->nTagType), to_swap);
    switch (tinfo->nTagType) {
    case ABF_VOICETAG:
        buf = read_int16p(buf, &(tinfo->nVoiceTagNumber), to_swap);
        break;
    case ABF_ANNOTATIONTAG:
        buf = read_int16p(buf, &(tinfo->nAnnotationIndex), to_swap);
        break;
    default:
        buf += 2;               /* skip */
        break;
    }
    return buf;                 /* total = 64 bytes */
}

char *abf_read_synch(char *buf, struct abf_synch *sinfo, bool to_swap)
{
    buf = read_int32p(buf, &(sinfo->lStart), to_swap);
    buf = read_int32p(buf, &(sinfo->lLength), to_swap);
    return buf;                 /* total = 8 bytes */
}

char *abf_read_voicetaginfo(char *buf, struct abf_voicetaginfo *vinfo, bool to_swap)
{
    buf = read_int32p(buf, &(vinfo->lTagNumber), to_swap);
    buf = read_int32p(buf, &(vinfo->lFileOffset), to_swap);
    buf = read_int32p(buf, &(vinfo->lUncompressedSize), to_swap);
    buf = read_int32p(buf, &(vinfo->lCompressedSize), to_swap);
    buf = read_int16p(buf, &(vinfo->nCompressionType), to_swap);
    buf = read_int16p(buf, &(vinfo->nSampleSize), to_swap);
    buf = read_int32p(buf, &(vinfo->lSamplesPerSecond), to_swap);
    buf = read_uint32p(buf, &(vinfo->dwCRC), to_swap);
    buf = read_uint16p(buf, &(vinfo->wChannels), to_swap);
    buf = read_uint16p(buf, &(vinfo->wUnused), to_swap);
    return buf;                 /* total = 32 bytes */
}

char *abf_read_delta(char *buf, struct abf_delta *dinfo, bool to_swap)
{
    buf = read_int32p(buf, &(dinfo->lDeltaTime), to_swap);
    buf = read_int32p(buf, &(dinfo->lParameterID), to_swap);
    buf = read_uint32p(buf, (uint32_t*)&(dinfo->lNewParamValue), to_swap);
    return buf;                 /* total = 12 bytes */
}
