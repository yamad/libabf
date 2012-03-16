#include <stdlib.h>

#include "abf2.h"

int abf2_read_guidp(char **bufp, struct GUID *guid, bool to_swap)
{
    int i;
    guid->Data1 = read_uint32p(bufp, to_swap);
    guid->Data2 = read_uint16p(bufp, to_swap);
    guid->Data3 = read_uint16p(bufp, to_swap);
    for (i = 0; i < 8; i++) {
        guid->Data4[i] = read_uint8p(bufp);
    }
}

int abf2_read_section(char **bufp, struct abf2_section **sec, bool to_swap)
{
    (*sec)->uBlockIndex = read_uint32p(bufp, to_swap);
    (*sec)->uBytes = read_uint32p(bufp, to_swap);
    (*sec)->llNumEntries = read_int64p(bufp, to_swap);
}

int abf2_read_fileinfo(char **bufp, struct abf2_fileinfo *finfo,  bool to_swap)
{
    /* deserialize structure members manually */
    finfo->uFileSignature = read_uint32p(bufp, to_swap);
    finfo->uFileVersionNumber = read_uint32p(bufp, to_swap);
    finfo->uFileInfoSize = read_uint32p(bufp, to_swap);
    finfo->uActualEpisodes = read_uint32p(bufp, to_swap);
    finfo->uFileStartDate = read_uint32p(bufp, to_swap);
    finfo->uFileStartTimeMS = read_uint32p(bufp, to_swap);
    finfo->uStopwatchTime = read_uint32p(bufp, to_swap);
    finfo->nFileType = read_int16p(bufp, to_swap);
    finfo->nDataFormat = read_int16p(bufp, to_swap);
    finfo->nSimultaneousScan = read_int16p(bufp, to_swap);
    finfo->nCRCEnable = read_int16p(bufp, to_swap);
    finfo->uFileCRC = read_uint32p(bufp, to_swap);
    abf2_read_guidp(bufp, &(finfo->FileGUID), to_swap);
    finfo->uCreatorVersion = read_uint32p(bufp, to_swap);
    finfo->uCreatorNameIndex = read_uint32p(bufp, to_swap);
    finfo->uModifierVersion = read_uint32p(bufp, to_swap);
    finfo->uModifierNameIndex = read_uint32p(bufp, to_swap);
    finfo->uProtocolPathIndex = read_uint32p(bufp, to_swap);

    abf2_read_section(bufp, &(finfo->ProtocolSection), to_swap);
    abf2_read_section(bufp, &(finfo->ADCSection), to_swap);
    abf2_read_section(bufp, &(finfo->DACSection), to_swap);
    abf2_read_section(bufp, &(finfo->EpochSection), to_swap);
    abf2_read_section(bufp, &(finfo->ADCPerDACSection), to_swap);
    abf2_read_section(bufp, &(finfo->EpochPerDACSection), to_swap);
    abf2_read_section(bufp, &(finfo->UserListSection), to_swap);
    abf2_read_section(bufp, &(finfo->StatsRegionSection), to_swap);
    abf2_read_section(bufp, &(finfo->MathSection), to_swap);
    abf2_read_section(bufp, &(finfo->StringsSection), to_swap);
    abf2_read_section(bufp, &(finfo->DataSection), to_swap);
    abf2_read_section(bufp, &(finfo->TagSection), to_swap);
    abf2_read_section(bufp, &(finfo->ScopeSection), to_swap);
    abf2_read_section(bufp, &(finfo->DeltaSection), to_swap);
    abf2_read_section(bufp, &(finfo->VoiceTagSection), to_swap);
    abf2_read_section(bufp, &(finfo->SynchArraySection), to_swap);
    abf2_read_section(bufp, &(finfo->AnnotationSection), to_swap);
    abf2_read_section(bufp, &(finfo->StatsSection), to_swap);
    /* ignore remaining 148 unused bytes read from buf */
}

int abf2_print_fileinfo(const struct abf2_fileinfo *finfo, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%20s:\t%lu\n", spaces, "uFileSignature", finfo->uFileSignature);
    printf("%s%20s:\t%lu\n", spaces, "uFileVersionNumber", finfo->uFileVersionNumber);
    printf("%s%20s:\t%lu\n", spaces, "uFileInfoSize", finfo->uFileInfoSize);
    printf("%s%20s:\t%lu\n", spaces, "uActualEpisodes", finfo->uActualEpisodes);
    printf("%s%20s:\t%lu\n", spaces, "uFileStartDate", finfo->uFileStartDate);
    printf("%s%20s:\t%lu\n", spaces, "uFileStartTimeMS", finfo->uFileStartTimeMS);
    printf("%s%20s:\t%lu\n", spaces, "uStopwatchTime", finfo->uStopwatchTime);
    printf("%s%20s:\t%d\n", spaces, "nFileType", finfo->nFileType);
    printf("%s%20s:\t%d\n", spaces, "nDataFormat", finfo->nDataFormat);
    printf("%s%20s:\t%d\n", spaces, "nSimultaneousScan", finfo->nSimultaneousScan);
    printf("%s%20s:\t%d\n", spaces, "nCRCEnable", finfo->nCRCEnable);
    printf("%s%20s:\t%lu\n", spaces, "uFileCRC", finfo->uFileCRC);
    printf("%s%20s:\t%lu\n", spaces, "uCreatorVersion", finfo->uCreatorVersion);
    printf("%s%20s:\t%lu\n", spaces, "uCreatorNameIndex", finfo->uCreatorNameIndex);
    printf("%s%20s:\t%lu\n", spaces, "uModifierVersion", finfo->uModifierVersion);
    printf("%s%20s:\t%lu\n", spaces, "uModifierNameIndex", finfo->uModifierNameIndex);
    printf("%s%20s:\t%lu\n", spaces, "uProtocolPathIndex", finfo->uProtocolPathIndex);

    printf("%s%20s:\n", spaces, "ProtocolSection");
    abf2_print_section(&(finfo->ProtocolSection), 4);
    printf("%s%20s:\n", spaces, "ADCSection");
    abf2_print_section(&(finfo->ADCSection), 4);
    printf("%s%20s:\n", spaces, "DACSection");
    abf2_print_section(&(finfo->DACSection), 4);
    printf("%s%20s:\n", spaces, "EpochSection");
    abf2_print_section(&(finfo->EpochSection), 4);
    printf("%s%20s:\n", spaces, "ADCPerDACSection");
    abf2_print_section(&(finfo->ADCPerDACSection), 4);
    printf("%s%20s:\n", spaces, "EpochPerDACSection");
    abf2_print_section(&(finfo->EpochPerDACSection), 4);
    printf("%s%20s:\n", spaces, "UserListSection");
    abf2_print_section(&(finfo->UserListSection), 4);
    printf("%s%20s:\n", spaces, "StatsRegionSection");
    abf2_print_section(&(finfo->StatsRegionSection), 4);
    printf("%s%20s:\n", spaces, "MathSection");
    abf2_print_section(&(finfo->MathSection), 4);
    printf("%s%20s:\n", spaces, "StringsSection");
    abf2_print_section(&(finfo->StringsSection), 4);
    printf("%s%20s:\n", spaces, "DataSection");
    abf2_print_section(&(finfo->DataSection), 4);
    printf("%s%20s:\n", spaces, "TagSection");
    abf2_print_section(&(finfo->TagSection), 4);
    printf("%s%20s:\n", spaces, "ScopeSection");
    abf2_print_section(&(finfo->ScopeSection), 4);
    printf("%s%20s:\n", spaces, "DeltaSection");
    abf2_print_section(&(finfo->DeltaSection), 4);
    printf("%s%20s:\n", spaces, "VoiceTagSection");
    abf2_print_section(&(finfo->VoiceTagSection), 4);
    printf("%s%20s:\n", spaces, "SynchArraySection");
    abf2_print_section(&(finfo->SynchArraySection), 4);
    printf("%s%20s:\n", spaces, "AnnotationSection");
    abf2_print_section(&(finfo->AnnotationSection), 4);
    printf("%s%20s:\n", spaces, "StatsSection");
    abf2_print_section(&(finfo->StatsSection), 4);
    free(spaces);
    return 0;
}

int abf2_print_section(const struct abf2_section *sec, int indent)
{
    char *spaces = get_repeated_string(' ', indent);
    printf("%s%13s: %lu\n", spaces, "uBlockIndex", sec->uBlockIndex);
    printf("%s%13s: %lu\n", spaces, "uBytes", sec->uBytes);
    printf("%s%13s: %llu\n", spaces, "llNumEntries", sec->llNumEntries);
    free(spaces);
    return 0;
}

char *get_repeated_string(char c, int repeats) {
    int i;
    char *head = malloc(repeats + 1);
    if (head == NULL) {
        printf("Memory failure\n");
        exit(1);
    }
    char *to = head;
    for (i = 0; i < repeats; i++) {
        *to = c;
    }
    *to = '\0';
    return head;
}

/* sloppy test function to print fileinfo struct */
int main(int argc, char *argv[])
{
    FILE *f;
    char *fname = argv[1];
    if (fname == NULL) fname = "";

    f = fopen(fname, "r");
    if (f == NULL) {
        fprintf(stderr, "Can't open file\n");
        exit(1);
    }

    char *buf = malloc(ABF2_FILEINFOSIZE);
    if (buf == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    if (0 == fread(buf, ABF2_FILEINFOSIZE, 1, f)) {
        printf("Read error\n");
        exit(1);
    }

    struct abf2_fileinfo *finfo;
    finfo = (struct abf2_fileinfo *) malloc(sizeof(struct abf2_fileinfo));
    int to_swap = 0;

    char **bufp = &buf;
    struct abf2_fileinfo **finfop = &finfo;

    abf2_read_fileinfo(bufp, &finfop, to_swap);
    abf2_print_fileinfo(finfo, 0);

    fclose(f);
    return 0;
}
