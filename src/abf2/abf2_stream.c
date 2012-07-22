#include <stdlib.h>
#include "abf2_stream.h"
#include "abf2_read.h"

StreamError abf2_stream_read_guid(stream_dt *stream, struct guid *guid, bool to_swap)
{
    uint8_t *buf = malloc(sizeof(struct guid));

    StreamError err = stream_read(stream, buf, sizeof(struct guid));
    if (StreamError_Success != err) return err;

    abf2_read_guidp(buf, guid, to_swap);
    free(buf);
    return err;
}
