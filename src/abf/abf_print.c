#include "abf_print.h"

int abf_print_logfont(const struct abf_logfont *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-15s: %d\n", spaces, "nHeight", s->nHeight);
    printf("%s%-15s: %d\n", spaces, "nWeight", s->nWeight);
    printf("%s%-15s: %#x\n", spaces, "cPitchAndFamily", s->cPitchAndFamily);
    printf("%s%-15s: %s\n", spaces, "szFaceName", (char*)s->szFaceName);
    free(spaces);
    return 0;
}

int abf_print_signal(const struct abf_signal *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-15s: %s\n", spaces, "szName", (char*)s->szName);
    printf("%s%-15s: %d\n", spaces, "nMxOffset", s->nMxOffset);
    printf("%s%-15s: %d\n", spaces, "rgbColor", s->rgbColor);
    printf("%s%-15s: %d\n", spaces, "nPenWidth", s->nPenWidth);
    printf("%s%-15s: %u\n", spaces, "bDrawPoints", s->bDrawPoints);
    printf("%s%-15s: %u\n", spaces, "bHidden", s->bHidden);
    printf("%s%-15s: %u\n", spaces, "bFloatData", s->bFloatData);
    printf("%s%-15s: %f\n", spaces, "fVertProportion", s->fVertProportion);
    printf("%s%-15s: %f\n", spaces, "fDisplayGain", s->fDisplayGain);
    printf("%s%-15s: %f\n", spaces, "fDisplayOffset", s->fDisplayOffset);
    free(spaces);
    return 0;
}

int abf_print_scopeconfig(const struct abf_scopeconfig *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    int i;
    printf("%s%-21s: %#x\n", spaces, "dwFlags", s->dwFlags);
    for (i=0; i<ABF_SCOPECOLORS; i++) {
        printf("%srgbColor[%d]%-10s: %#x\n", spaces, i, " ", s->rgbColor[i]);
    }
    printf("%s%-21s: %f\n", spaces, "fDisplayStart", s->fDisplayStart);
    printf("%s%-21s: %f\n", spaces, "fDisplayEnd", s->fDisplayEnd);
    printf("%s%-21s: %u\n", spaces, "wScopeMode", s->wScopeMode);
    printf("%s%-21s: %u\n", spaces, "bMaximized", s->bMaximized);
    printf("%s%-21s: %u\n", spaces, "bMinimized", s->bMinimized);
    printf("%s%-21s: %d\n", spaces, "xLeft", s->xLeft);
    printf("%s%-21s: %d\n", spaces, "yTop", s->yTop);
    printf("%s%-21s: %d\n", spaces, "xRight", s->xRight);
    printf("%s%-21s: %d\n", spaces, "yBottom", s->yBottom);
    printf("%sLogFont: \n", spaces);
    abf_print_logfont(&(s->LogFont), indent+6);
    printf("%sTraceLists: \n", spaces);
    for (i=0; i<s->nTraceCount; i++) {
        printf("%s  ADC #%d: \n", spaces, i);
        abf_print_signal(&(s->TraceList[i]), indent+6);
    }
    printf("%s%-21s: %d\n", spaces, "nYAxisWidth", s->nYAxisWidth);
    printf("%s%-21s: %d\n", spaces, "nTraceCount", s->nTraceCount);
    printf("%s%-21s: %d\n", spaces, "nEraseStrategy", s->nEraseStrategy);
    printf("%s%-21s: %d\n", spaces, "nDockState", s->nDockState);

    /* Section 2 scope configurations for file version 1.68. */
    printf("%s%-21s: %d\n", spaces, "nSizeofOldStructure", s->nSizeofOldStructure);
    for (i=0; i<ABF_SCOPECOLORS_EX; i++) {
        printf("%srgbColorEx[%2.2d]%-7s: %#x\n", spaces, i, " ", s->rgbColorEx[i]);
    }
    printf("%s%-21s: %d\n", spaces, "nAutoZeroState", s->nAutoZeroState);
    printf("%s%-21s: %#x\n", spaces, "dwCursorsVisibleState", s->dwCursorsVisibleState);
    printf("%s%-21s: %#x\n", spaces, "dwCursorsLockedState", s->dwCursorsLockedState);
    free(spaces);
    return 0;
}

int abf_print_tag(const struct abf_tag *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-16s: %d\n", spaces, "lTagTime", s->lTagTime);
    printf("%s%-16s: %s\n", spaces, "sComment", (char*)s->sComment);
    printf("%s%-16s: %d\n", spaces, "nTagType", s->nTagType);
    switch (s->nTagType) {
    case ABF_TAGTYPE_VOICE:
        printf("%s%-16s: %d\n", spaces, "nVoiceTagNumber", s->nVoiceTagNumber);
        break;
    case ABF_TAGTYPE_ANNOTATION:
        printf("%s%-16s: %d\n", spaces, "nAnnotationIndex", s->nAnnotationIndex);
        break;
    }
    free(spaces);
    return 0;
}

int abf_print_synch(const struct abf_synch *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-7s: %d\n", spaces, "lStart", s->lStart);
    printf("%s%-7s: %d\n", spaces, "lLength", s->lLength);
    free(spaces);
    return 0;
}

int abf_print_voicetaginfo(const struct abf_voicetaginfo *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-17s: %d\n", spaces, "lTagNumber", s->lTagNumber);
    printf("%s%-17s: %d\n", spaces, "lFileOffset", s->lFileOffset);
    printf("%s%-17s: %d\n", spaces, "lUncompressedSize", s->lUncompressedSize);
    printf("%s%-17s: %d\n", spaces, "lCompressedSize", s->lCompressedSize);
    printf("%s%-17s: %d\n", spaces, "nCompressionType", s->nCompressionType);
    printf("%s%-17s: %d\n", spaces, "nSampleSize", s->nSampleSize);
    printf("%s%-17s: %d\n", spaces, "lSamplesPerSecond", s->lSamplesPerSecond);
    printf("%s%-17s: %#x\n", spaces, "dwCRC", s->dwCRC);
    printf("%s%-17s: %d\n", spaces, "wChannels", s->wChannels);
    free(spaces);
    return 0;
}

int abf_print_delta(const struct abf_delta *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-14s: %d\n", spaces, "lDeltaTime", s->lDeltaTime);
    printf("%s%-14s: %d\n", spaces, "lParameterID", s->lParameterID);
    /* TODO: figure out which NewParamValue to show.
     *
     * docs say that the value depends on the value of lParameterID,
     * but doesn't say how.
     */
    printf("%s%-14s: %d\n", spaces, "lNewParamValue", s->lNewParamValue);
    printf("%s%-14s: %f\n", spaces, "fNewParamValue", s->fNewParamValue);
    free(spaces);
    return 0;
}

int abf_print_stringcacheheader(const struct abf_stringcacheheader *s, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%-11s: %s\n", spaces, "dwSignature", (char*)&(s->dwSignature));
    printf("%s%-11s: %u\n", spaces, "dwVersion", s->dwVersion);
    printf("%s%-11s: %u\n", spaces, "uNumStrings", s->uNumStrings);
    printf("%s%-11s: %u\n", spaces, "uMaxSize", s->uMaxSize);
    printf("%s%-11s: %d\n", spaces, "lTotalBytes", s->lTotalBytes);
    free(spaces);
    return 0;
}
