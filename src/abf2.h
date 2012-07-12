#ifndef ABF2_H
#define ABF2_H

#include <stdio.h>

#include "deserialize.h"
#include "abf2_struct.h"

#define ABF2_BLOCKSIZE 512

#define ABF2_SECTIONSIZE 16
#define ABF2_FILEINFOSIZE 512
#define ABF2_PROTOCOLINFOSIZE 512
#define ABF2_MATHINFOSIZE 128

int abf2_needs_swap(const char *buf);

size_t abf2_section_block_offset(const struct abf2_section *sec);
size_t abf2_get_block_offset(uint32_t block);

char *abf2_read_guidp(char *buf, struct guid *guid, bool to_swap);
char *abf2_read_sectionp(char *buf, struct abf2_section *sec, bool to_swap);
char *abf2_read_fileinfo(char *buf, struct abf2_fileinfo *finfo, bool to_swap);
char *abf2_read_protocolinfo(char *buf, struct abf2_protocolinfo *pinfo, bool to_swap);

#endif
