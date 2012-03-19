#ifndef ABF2_H
#define ABF2_H

#include <stdio.h>

#include "deserialize.h"
#include "abf2_struct.h"

#define ABF2_BLOCKSIZE 512

#define ABF2_SECTIONSIZE 16
#define ABF2_FILEINFOSIZE 512

char *abf2_read_guidp(char *buf, struct guid *guid, bool to_swap);
char *abf2_read_sectionp(char *buf, struct abf2_section *sec, bool to_swap);
int abf2_read_fileinfo(char *buf, struct abf2_fileinfo *finfo, bool to_swap);

int abf2_print_fileinfo(const struct abf2_fileinfo *finfo, int indent);
int abf2_print_section(const struct abf2_section *sec, int indent);
char *get_repeated_string(char c, int repeats);

#endif
