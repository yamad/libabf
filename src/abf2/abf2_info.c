#include <stdint.h>

#include "memory.h"
#include "abf_string.h"
#include "abf_read.h"
#include "abf_print.h"
#include "abf2_print.h"

int abf2_check_section_data_exists(const struct abf2_section *sec);
int64_t abf2_select_section_entry(const struct abf2_section *sec,
                                  char *sec_string);
int abf2_seek_to_section_entry(FILE *f, const struct abf2_section *sec,
                               char *sec_string);
int abf2_section_seek_entry(FILE *f, const struct abf2_section *sec,
                            int64_t entry_num);
int abf2_section_load_entry(FILE *f, const struct abf2_section *sec,
                            char *buf, int64_t entry_num);
int abf2_display_cleanup(const struct abf2_section *sec, int64_t *eselect);

int abf2_display_protocolinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_adcinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_dacinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_epochinfoperdac(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_epochinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_statsregioninfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_userlistinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_stringcache(FILE* f, const struct abf2_section *sec, bool to_swap);

int abf2_display_scopeconfigs(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_tags(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_deltas(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_voicetags(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_synchs(FILE* f, const struct abf2_section *sec, bool to_swap);

enum ABF2SELECT_OPTIONS {
    ABF2SELECT_FileInfo = 1,
    ABF2SELECT_ProtocolInfo = 2,
    ABF2SELECT_ADCInfo = 3,
    ABF2SELECT_DACInfo = 4,
    ABF2SELECT_EpochInfoPerDAC = 5,
    ABF2SELECT_EpochInfo = 6,
    ABF2SELECT_UserListInfo = 7,
    ABF2SELECT_StatsRegionInfo = 8,
    ABF2SELECT_MathInfo = 9,
    ABF2SELECT_Tags = 10,
    ABF2SELECT_Scopes = 11,
    ABF2SELECT_Deltas = 12,
    ABF2SELECT_VoiceTags = 13,
    ABF2SELECT_Synchs = 14,
    ABF2SELECT_Annotations = 15,
    ABF2SELECT_Stats = 16,
    ABF2SELECT_Strings = 17
};

char *ABF2_SELECTMENU = \
    "MAIN MENU\n\n"
    " ABF2 structs\n"
    " ----------------------\n"
    " [1] FileInfo        [2] ProtocolInfo     [3] ADCInfo\n"
    " [4] DACInfo         [5] EpochInfo        [6] EpochInfoPerDAC\n"
    " [7] UserListInfo    [8] StatsRegionInfo  [9] MathInfo\n\n"
    " ABF structs\n"
    " ----------------------\n"
    " [10] Tags           [11] Scopes          [12] Deltas\n"
    " [13] VoiceTags      [14] Synchs          [15] Annotations\n"
    " [16] StatsConfig    [17] Strings\n\n"
    "Select a section to show [1-17] (or 0 to exit): ";

char buf[2048];                 /* TODO: lazy alert -- global buffer */

int abf2_check_section_data_exists(const struct abf2_section *sec)
{
    if (sec->uBlockIndex == 0 || sec->llNumEntries == 0) {
        printf("No info for this section in this file\n\n");
        return 0;
    }
    return 1;
}

int64_t abf2_select_section_entry(const struct abf2_section *sec, char *sec_string)
{
    int64_t esel = -1;
    int64_t nentries = sec->llNumEntries;
    if (nentries < 1) {
        return 0;               /* no entries, bail */
    } else if (nentries == 1) {
        return 1;               /* one entry, so it must be selected */
    } else {
        while (esel == -1) {
            printf("\nSelect an entry to show [1-%lld] (or 0 to exit): ",
                   nentries, sec_string, nentries);
            scanf("%lld", &esel); /* TODO: stop using scanf */
            if (esel < 0 || esel > nentries) {/* error check */
                printf("\nOut of range. Select a number between 1 and %lld", nentries);
                esel = -1;
            }
            printf("\n");
        }
        return esel;
    }
}

int abf2_seek_to_section_entry(FILE *f, const struct abf2_section *sec, char *sec_string)
{
    int64_t eselect = abf2_select_section_entry(sec, sec_string);
    if (eselect == 0) return 0;
    abf2_section_seek_entry(f, sec, eselect);
    return eselect;
}

#define GETENTRY_TOOBIG 1
#define GETENTRY_READERR 2

/* seek file to the given entry */
int abf2_section_seek_entry(FILE *f,
                            const struct abf2_section *sec,
                            int64_t entry_num)
{
    if (sec->llNumEntries < entry_num)
        return GETENTRY_TOOBIG;
    size_t block_offset = abf2_section_block_offset(sec);
    size_t entry_offset = sec->uBytes * (entry_num);
    fseek(f, block_offset + entry_offset, SEEK_SET);
    return 0;
}

/* load data for given entry into `buf` */
int abf2_section_load_entry(FILE *f,
                            const struct abf2_section *sec,
                            char *buf,
                            int64_t entry_num)
{
    int err = 0;
    size_t bytes_read;

    err = abf2_section_seek_entry(f, sec, entry_num);
    if (0 != err)
        return err;

    bytes_read = fread(buf, (size_t)sec->uBytes, 1, f);
    if (0 == bytes_read)
        return GETENTRY_READERR;

    return err;
}

int abf2_display_cleanup(const struct abf2_section *sec, int64_t *eselect)
{
    if (sec->llNumEntries == 1)
        *eselect = 0;
    else {
        *eselect = -1;
        printf("---------------------------------------------\n");
    }
    return 0;
}

int abf2_display_protocolinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    int err = 0;
    int64_t eselect = -1;
    struct abf2_protocolinfo *info;
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    info = malloc(sizeof(struct abf2_protocolinfo));

    while (eselect != 0) {
        eselect = abf2_select_section_entry(sec, "ProtocolInfo");
        if (eselect == 0)
            return 0; /* exit on selecting 0 */

        err = abf2_section_load_entry(f, sec, buf, eselect-1);
        if (0 != err) {
            printf("Read entry error\n");
            return err;
        }
        abf2_read_protocolinfo(buf, info, to_swap);
        printf("\n");
        abf2_print_protocolinfo(info, 0);
        printf("\n");
        abf2_display_cleanup(sec, &eselect);
    }
    free(info); info = NULL;
    return err;
}

int abf2_display_adcinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    int err;
    int64_t eselect = -1;
    struct abf2_adcinfo *info;
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    info = malloc(sizeof(struct abf2_adcinfo));

    while (eselect != 0) {
        eselect = abf2_select_section_entry(sec, "ADCInfo");
        if (eselect == 0)
            goto clean_exit;

        err = abf2_section_load_entry(f, sec, buf, eselect-1);
        if (0 != err) {
            printf("Read entry error\n");
            return err;
        }
        abf2_read_adcinfo(buf, info, to_swap);
        printf("\n");
        abf2_print_adcinfo(info, 0);
        printf("\n");
        abf2_display_cleanup(sec, &eselect);
    }
clean_exit:
    free(info);
    info = NULL;
    return 0;
}

int abf2_display_dacinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    struct abf2_dacinfo *ainfo;
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    ainfo = (struct abf2_dacinfo *) malloc(sizeof(struct abf2_dacinfo));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "DACInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF2_DACINFOSIZE, 1, f)) {
            printf("Read DAC info error\n");
            return -1;
        }
        abf2_read_dacinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_dacinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(ainfo);
    return 0;
}

int abf2_display_epochinfoperdac(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    struct abf2_epochinfoperdac *ainfo;
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    ainfo = (struct abf2_epochinfoperdac *) malloc(sizeof(struct abf2_epochinfoperdac));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "EpochInfoPerDAC");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF2_EPOCHINFOPERDACSIZE, 1, f)) {
            printf("Read DAC info error\n");
            return -1;
        }
        abf2_read_epochinfoperdac(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_epochinfoperdac(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(ainfo);
    return 0;
}

int abf2_display_epochinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf2_epochinfo *ainfo;
    ainfo = (struct abf2_epochinfo *) malloc(sizeof(struct abf2_epochinfo));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "EpochInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF2_EPOCHINFOSIZE, 1, f)) {
            printf("Read DAC info error\n");
            return -1;
        }
        abf2_read_epochinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_epochinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(ainfo);
    return 0;
}

int abf2_display_statsregioninfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf2_statsregioninfo *ainfo;
    ainfo = (struct abf2_statsregioninfo *) malloc(sizeof(struct abf2_statsregioninfo));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "StatsRegionInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */
        if (0 == fread(buf, ABF2_STATSREGIONINFOSIZE, 1, f)) {
            printf("Read StatsRegion info error\n");
            return -1;
        }
        abf2_read_statsregioninfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_statsregioninfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(ainfo);
    return 0;
}

int abf2_display_userlistinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf2_userlistinfo *ainfo;
    ainfo = (struct abf2_userlistinfo *) malloc(sizeof(struct abf2_userlistinfo));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "UserListInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF2_USERLISTINFOSIZE, 1, f)) {
            printf("Read UserList info error\n");
            return -1;
        }
        abf2_read_userlistinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_userlistinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(ainfo);
    return 0;
}

int abf2_display_mathinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf2_mathinfo *ainfo;
    ainfo = (struct abf2_mathinfo *) malloc(sizeof(struct abf2_mathinfo));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "MathInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF2_MATHINFOSIZE, 1, f)) {
            printf("Read Math info error\n");
            return -1;
        }
        abf2_read_mathinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_mathinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(ainfo);
    return 0;
}

int abf2_display_tags(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf_tag *tag;
    tag = (struct abf_tag *) malloc(sizeof(struct abf_tag));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "Tags");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF_TAGSIZE, 1, f)) {
            printf("Read Tags error\n");
            return -1;
        }
        abf_read_tag(buf, tag, to_swap);
        printf("\n");
        abf_print_tag(tag, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(tag);
    return 0;
}

int abf2_display_scopeconfigs(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf_scopeconfig *sc;
    sc = (struct abf_scopeconfig *) malloc(sizeof(struct abf_scopeconfig));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "Scope Configs");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF_SCOPECONFIGSIZE, 1, f)) {
            printf("Read Scope Config error\n");
            return -1;
        }
        abf_read_scopeconfig(buf, sc, to_swap);
        printf("\n");
        abf_print_scopeconfig(sc, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(sc);
    return 0;
}

int abf2_display_deltas(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf_delta *delta;
    delta = (struct abf_delta *) malloc(sizeof(struct abf_delta));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "Deltas");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF_DELTASIZE, 1, f)) {
            printf("Read Deltas error\n");
            return -1;
        }
        abf_read_delta(buf, delta, to_swap);
        printf("\n");
        abf_print_delta(delta, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(delta);
    return 0;
}

int abf2_display_voicetags(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf_voicetaginfo *voicetag;
    voicetag = (struct abf_voicetaginfo *) malloc(sizeof(struct abf_voicetaginfo));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "Voice Tags");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */
        if (0 == fread(buf, ABF_VOICETAGINFOSIZE, 1, f)) {
            printf("Read Voice Tags error\n");
            return -1;
        }
        abf_read_voicetaginfo(buf, voicetag, to_swap);
        printf("\n");
        abf_print_voicetaginfo(voicetag, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(voicetag);
    return 0;
}

int abf2_display_synchs(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    struct abf_synch *synch;
    synch = (struct abf_synch *) malloc(sizeof(struct abf_synch));

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "Synchs");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        if (0 == fread(buf, ABF_SYNCHSIZE, 1, f)) {
            printf("Read Synchs error\n");
            return -1;
        }
        abf_read_synch(buf, synch, to_swap);
        printf("\n");
        abf_print_synch(synch, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    free(synch);
    return 0;
}

int abf2_display_stringcache(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    int err = 0;
    int64_t eselect = -1;
    struct abf_stringcacheheader *sch;
    struct abf_strcache *sc;
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }
    sch = malloc(sizeof(struct abf_stringcacheheader));
    sc = malloc(sizeof(struct abf_strcache));
    abf_strcache_init(sc);

    abf2_section_load_entry(f, sec, buf, 0);
    abf_read_stringcache(buf, sch, sc, to_swap);
    printf("\n");
    abf_print_stringcacheheader(sch, 0);
    printf("\n");
    abf_strcache_printall(sc, 0);
    printf("\n");
    abf2_display_cleanup(sec, &eselect);

    abf_strcache_destroy(sc);
    free(sc); sc = NULL;
    free(sch); sch = NULL;
    return err;
}

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    char *fname = argv[1];
    if (fname == NULL) fname = "";

    printf("\nABF2 (Axon Binary Format v2) File Reader\n");
    printf("=============================================\n\n");

    printf("Active File: %s\n\n", fname);

    f = fopen(fname, "r");
    if (f == NULL) {
        fprintf(stderr, "Can't open the given file. Check file path.\n");
        exit(1);
    }

    struct abf2_fileinfo *finfo;
    finfo = (struct abf2_fileinfo *) malloc(sizeof(struct abf2_fileinfo));
    if (finfo == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    if (0 == fread(buf, ABF2_FILEINFOSIZE, 1, f)) {
        printf("Read error. Check that the file is an ABF2 file?\n");
        exit(1);
    }

    if (0 == abf2_can_open(buf)) {
        printf("Can't read non-ABF2 files.\n");
        exit(1);
    }

    int to_swap = abf2_needs_swap(buf);
    abf2_read_fileinfo(buf, finfo, to_swap);

    int struct_select = -1;
    struct abf2_section *sec;
    while (struct_select != 0) {
        printf("=============================================\n\n");
        printf("%s", ABF2_SELECTMENU);
        /* TODO: laziness alert -- using scanf */
        scanf("%d", &struct_select);

        if (struct_select == 0) exit(0);
        printf("\n=============================================\n\n");
        switch(struct_select) {
        case ABF2SELECT_FileInfo:
            printf("FileInfo\n");
            abf2_print_fileinfo(finfo, 2);
            break;
        case ABF2SELECT_ProtocolInfo:
            sec = &(finfo->ProtocolSection);
            printf("ProtocolSection\n");
            abf2_print_section(sec, 2);
            abf2_display_protocolinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_ADCInfo:
            sec = &(finfo->ADCSection);
            printf("ADCSection\n");
            abf2_print_section(sec, 2);
            abf2_display_adcinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_DACInfo:
            sec = &(finfo->DACSection);
            printf("DACSection\n");
            abf2_print_section(sec, 2);
            abf2_display_dacinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_EpochInfoPerDAC:
            sec = &(finfo->EpochPerDACSection);
            printf("EpochPerDACSection\n");
            abf2_print_section(sec, 2);
            abf2_display_epochinfoperdac(f, sec, to_swap);
            break;
        case ABF2SELECT_EpochInfo:
            sec = &(finfo->EpochSection);
            printf("EpochSection\n");
            abf2_print_section(sec, 2);
            abf2_display_epochinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_StatsRegionInfo:
            sec = &(finfo->StatsRegionSection);
            printf("StatsRegionSection\n");
            abf2_print_section(sec, 2);
            abf2_display_statsregioninfo(f, sec, to_swap);
            break;
        case ABF2SELECT_UserListInfo:
            sec = &(finfo->UserListSection);
            printf("UserListSection\n");
            abf2_print_section(sec, 2);
            abf2_display_userlistinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_MathInfo:
            sec = &(finfo->MathSection);
            printf("MathSection\n");
            abf2_print_section(sec, 2);
            abf2_display_mathinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_Tags:
            sec = &(finfo->TagSection);
            printf("TagSection\n");
            abf2_print_section(sec, 2);
            abf2_display_tags(f, sec, to_swap);
            break;
        case ABF2SELECT_Scopes:
            sec = &(finfo->ScopeSection);
            printf("ScopeSection\n");
            abf2_print_section(sec, 2);
            abf2_display_scopeconfigs(f, sec, to_swap);
            break;
        case ABF2SELECT_Deltas:
            sec = &(finfo->DeltaSection);
            printf("DeltaSection\n");
            abf2_print_section(sec, 2);
            abf2_display_deltas(f, sec, to_swap);
            break;
        case ABF2SELECT_VoiceTags:
            sec = &(finfo->VoiceTagSection);
            printf("VoiceTagSection\n");
            abf2_print_section(sec, 2);
            abf2_display_voicetags(f, sec, to_swap);
            break;
        case ABF2SELECT_Synchs:
            sec = &(finfo->SynchArraySection);
            printf("SynchArraySection\n");
            abf2_print_section(sec, 2);
            abf2_display_synchs(f, sec, to_swap);
            break;
        case ABF2SELECT_Annotations:
            sec = &(finfo->AnnotationSection);
            printf("AnnotationSection\n");
            abf2_print_section(sec, 2);
            abf2_display_stringcache(f, sec, to_swap);
            break;
        case ABF2SELECT_Stats:
            sec = &(finfo->StatsSection);
            printf("StatsSection\n");
            abf2_print_section(sec, 2);
            abf2_display_scopeconfigs(f, sec, to_swap);
            break;
        case ABF2SELECT_Strings:
            sec = &(finfo->StringsSection);
            printf("StringsSection\n");
            abf2_print_section(sec, 2);
            abf2_display_stringcache(f, sec, to_swap);
            break;
        default:
            printf("Unknown option. Try again, please.\n\n");
            break;
        }
    }

    free(finfo);
    fclose(f);
    return 0;
 }
