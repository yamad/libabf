#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printutil.h"
#include "abf2_read.h"

int abf2_print_guid(const struct guid *guid, int indent);
int abf2_print_section(const struct abf2_section *sec, int indent);
int abf2_print_fileinfo(const struct abf2_fileinfo *finfo, int indent);
int abf2_print_protocolinfo(const struct abf2_protocolinfo *pinfo, int indent);
int abf2_print_mathinfo(const struct abf2_mathinfo *minfo, int indent);
int abf2_print_adcinfo(const struct abf2_adcinfo *ainfo, int indent);
int abf2_print_dacinfo(const struct abf2_dacinfo *dinfo, int indent);
int abf2_print_epochinfoperdac(const struct abf2_epochinfoperdac *einfo, int indent);
int abf2_print_epochinfo(const struct abf2_epochinfo *einfo, int indent);
int abf2_print_statsregioninfo(const struct abf2_statsregioninfo *sinfo, int indent);
int abf2_print_userlistinfo(const struct abf2_userlistinfo *uinfo, int indent);
