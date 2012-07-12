#include "abf2_print.h"

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

    int to_swap = abf2_needs_swap(buf);
    abf2_read_fileinfo(buf, finfo, to_swap);
    abf2_print_fileinfo(finfo, 0);

    fclose(f);
    return 0;
}
