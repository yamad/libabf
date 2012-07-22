#ifndef ABF2_H
#define ABF2_H

#include <stdio.h>
#include <stdint.h>

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

int abf2_can_open(const uint8_t *buf);
int abf2_needs_swap(const uint8_t *buf);

size_t abf2_section_block_offset(const struct abf2_section *sec);
size_t abf2_get_block_offset(uint32_t block);

uint8_t *abf2_read_guidp(uint8_t *buf, struct guid *guid, bool to_swap);

uint8_t *abf2_read_sectionp(uint8_t *buf, struct abf2_section *sec,
                            bool to_swap);

uint8_t *abf2_read_fileinfo(uint8_t *buf, struct abf2_fileinfo *finfo,
                            bool to_swap);

uint8_t *abf2_read_protocolinfo(uint8_t *buf,
                                struct abf2_protocolinfo *pinfo,
                                bool to_swap);

uint8_t *abf2_read_mathinfo(uint8_t *buf, struct abf2_mathinfo *minfo,
                            bool to_swap);

uint8_t *abf2_read_adcinfo(uint8_t *buf, struct abf2_adcinfo *ainfo,
                           bool to_swap);

uint8_t *abf2_read_dacinfo(uint8_t *buf, struct abf2_dacinfo *dinfo,
                           bool to_swap);

uint8_t *abf2_read_epochinfoperdac(uint8_t *buf,
                                   struct abf2_epochinfoperdac *einfo,
                                   bool to_swap);

uint8_t *abf2_read_epochinfo(uint8_t *buf,
                             struct abf2_epochinfo *einfo,
                             bool to_swap);

uint8_t *abf2_read_statsregioninfo(uint8_t *buf,
                                   struct abf2_statsregioninfo *sinfo,
                                   bool to_swap);

uint8_t *abf2_read_userlistinfo(uint8_t *buf,
                                struct abf2_userlistinfo *uinfo,
                                bool to_swap);

int abf2_verify_filesignature(uint32_t filesig);

#endif
