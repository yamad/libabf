#ifndef ABF2_H
#define ABF2_H

#include <stdio.h>

#include "deserialize.h"
#include "abf2_struct.h"

#define ABF2_BLOCKSIZE 512

#define GUIDSIZE 16
#define ABF2_SECTIONSIZE 16
#define ABF2_FILEINFOSIZE 512
#define ABF2_PROTOCOLINFOSIZE 512
#define ABF2_MATHINFOSIZE 128
#define ABF2_ADCINFOSIZE 128
#define ABF2_DACINFOSIZE 256
#define ABF2_EPOCHINFOPERDACSIZE 48
#define ABF2_EPOCHINFOSIZE 32
#define ABF2_STATSREGIONINFOSIZE 128
#define ABF2_USERLISTINFOSIZE 64

int abf2_can_open(const char *buf);
int abf2_needs_swap(const char *buf);

size_t abf2_section_block_offset(const struct abf2_section *sec);
size_t abf2_get_block_offset(uint32_t block);

char *abf2_read_guidp(char *buf, struct guid *guid, bool to_swap);
char *abf2_read_sectionp(char *buf, struct abf2_section *sec, bool to_swap);

char *abf2_read_fileinfo(char *buf, struct abf2_fileinfo *finfo, bool to_swap);
char *abf2_read_protocolinfo(char *buf, struct abf2_protocolinfo *pinfo, bool to_swap);
char *abf2_read_mathinfo(char *buf, struct abf2_mathinfo *minfo, bool to_swap);
char *abf2_read_adcinfo(char *buf, struct abf2_adcinfo *ainfo, bool to_swap);
char *abf2_read_dacinfo(char *buf, struct abf2_dacinfo *dinfo, bool to_swap);
char *abf2_read_epochinfoperdac(char *buf, struct abf2_epochinfoperdac *einfo, bool to_swap);
char *abf2_read_epochinfo(char *buf, struct abf2_epochinfo *einfo, bool to_swap);
char *abf2_read_statsregioninfo(char *buf, struct abf2_statsregioninfo *sinfo, bool to_swap);
char *abf2_read_userlistinfo(char *buf, struct abf2_userlistinfo *uinfo, bool to_swap);

int abf2_verify_filesignature(uint32_t filesig);

#endif
