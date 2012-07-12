#ifndef ABF_READ_H
#define ABF_READ_H

#include <stdio.h>

#include "deserialize.h"
#include "abf_struct.h"

char *abf_read_logfont(char *buf, struct abf_logfont *linfo, bool to_swap);
char *abf_read_signal(char *buf, struct abf_signal *sinfo, bool to_swap);
char *abf_read_scopeconfig(char *buf, struct abf_scopeconfig *sinfo, bool to_swap);
char *abf_read_tag(char *buf, struct abf_tag *tinfo, bool to_swap);
char *abf_read_synch(char *buf, struct abf_synch *sinfo, bool to_swap);
char *abf_read_voicetaginfo(char *buf, struct abf_voicetaginfo *vinfo, bool to_swap);
char *abf_read_delta(char *buf, struct abf_delta *dinfo, bool to_swap);

#endif /* ABF_READ_H */
