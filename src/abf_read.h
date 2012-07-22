#ifndef ABF_READ_H
#define ABF_READ_H

#include <stdio.h>

#include "deserialize.h"
#include "abf_struct.h"

uint8_t *abf_read_logfont(uint8_t *buf, struct abf_logfont *linfo, bool to_swap);
uint8_t *abf_read_signal(uint8_t *buf, struct abf_signal *sinfo, bool to_swap);
uint8_t *abf_read_scopeconfig(uint8_t *buf, struct abf_scopeconfig *sinfo, bool to_swap);
uint8_t *abf_read_tag(uint8_t *buf, struct abf_tag *tinfo, bool to_swap);
uint8_t *abf_read_synch(uint8_t *buf, struct abf_synch *sinfo, bool to_swap);
uint8_t *abf_read_voicetaginfo(uint8_t *buf, struct abf_voicetaginfo *vinfo, bool to_swap);
uint8_t *abf_read_delta(uint8_t *buf, struct abf_delta *dinfo, bool to_swap);

#endif /* ABF_READ_H */
