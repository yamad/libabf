#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abf2.h"

char *get_repeated_string(char c, int repeats);

int abf2_print_guid(const struct guid *guid, int indent);
int abf2_print_section(const struct abf2_section *sec, int indent);
int abf2_print_fileinfo(const struct abf2_fileinfo *finfo, int indent);
int abf2_print_protocolinfo(const struct abf2_protocolinfo *pinfo, int indent);
