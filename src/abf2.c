#include <stdlib.h>
#include <string.h>

#include "abf2.h"

char *abf2_read_guidp(char *buf, struct guid *guid, bool to_swap)
{
    int i;
    buf = read_uint32p(buf, &(guid->Data1), to_swap);
    buf = read_uint16p(buf, &(guid->Data2), to_swap);
    buf = read_uint16p(buf, &(guid->Data3), to_swap);
    for (i = 0; i < 8; i++) {
        buf = read_uint8p(buf, &(guid->Data4[i]));
    }
    return buf;
}

char *abf2_read_sectionp(char *buf, struct abf2_section *sec, bool to_swap)
{
    buf = read_uint32p(buf, &(sec->uBlockIndex), to_swap);
    buf = read_uint32p(buf, &(sec->uBytes), to_swap);
    buf = read_int64p(buf, &(sec->llNumEntries), to_swap);
    return buf;
}

int abf2_read_fileinfo(char *buf, struct abf2_fileinfo *finfo,  bool to_swap)
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
    /* ignore remaining 148 unused bytes read from buf */
    return 0;
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

/* sloppy test function to print fileinfo struct */
/* int main(int argc, char *argv[]) */
/* { */
/*     FILE *f; */
/*     char *fname = argv[1]; */
/*     if (fname == NULL) fname = ""; */

/*     f = fopen(fname, "r"); */
/*     if (f == NULL) { */
/*         fprintf(stderr, "Can't open file\n"); */
/*         exit(1); */
/*     } */

/*     char *buf = malloc(ABF2_FILEINFOSIZE); */
/*     if (buf == NULL) { */
/*         printf("Memory allocation error\n"); */
/*         exit(1); */
/*     } */
/*     if (0 == fread(buf, ABF2_FILEINFOSIZE, 1, f)) { */
/*         printf("Read error\n"); */
/*         exit(1); */
/*     } */

/*     struct abf2_fileinfo *finfo; */
/*     finfo = (struct abf2_fileinfo *) malloc(sizeof(struct abf2_fileinfo)); */
/*     int to_swap = 0; */

/*     abf2_read_fileinfo(buf, finfo, to_swap); */
/*     abf2_print_fileinfo(finfo, 0); */

/*     fclose(f); */
/*     return 0; */
/* } */
