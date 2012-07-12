#include <stdlib.h>
#include "abf2.h"

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

    struct abf2_protocolinfo *pinfo;
    pinfo = (struct abf2_protocolinfo *) malloc(sizeof(struct abf2_protocolinfo));

    int to_swap = abf2_needs_swap(buf);

    abf2_read_fileinfo(buf, finfo, to_swap);

    fseek(f, abf2_section_block_offset(&(finfo->ProtocolSection)), SEEK_SET);
    if (0 == fread(buf, finfo->ProtocolSection.uBytes, finfo->ProtocolSection.llNumEntries, f)) {
        printf("Read protocol info error\n");
        exit(1);
    }
    abf2_read_protocolinfo(buf, pinfo, to_swap);
    abf2_print_protocolinfo(pinfo, 0);

    fclose(f);
    return 0;
}
