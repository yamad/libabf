#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printutil.h"
#include "abf_struct.h"

int abf_print_logfont(const struct abf_logfont *linfo, int indent);
int abf_print_signal(const struct abf_signal *sinfo, int indent);
int abf_print_scopeconfig(const struct abf_scopeconfig *sinfo, int indent);
int abf_print_tag(const struct abf_tag *tinfo, int indent);
int abf_print_synch(const struct abf_synch *sinfo, int indent);
int abf_print_voicetaginfo(const struct abf_voicetaginfo *vinfo, int indent);
int abf_print_delta(const struct abf_delta *dinfo, int indent);
int abf_print_stringcacheheader(const struct abf_stringcacheheader *sch, int indent);
