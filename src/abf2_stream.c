#include <stdlib.h>
#include "abf2_stream.h"

StreamError abf2_stream_read_guid(stream_dt *stream, struct guid *guid, bool to_swap)
{
    int i;
    StreamError err;
    char *buf = malloc(sizeof(struct guid));
    char *head = buf;

    err = stream_read(stream, buf, sizeof(struct guid));
    if (StreamError_Success != err) return err;

    buf = read_uint32p(buf, &(guid->Data1), to_swap);
    buf = read_uint16p(buf, &(guid->Data2), to_swap);
    buf = read_uint16p(buf, &(guid->Data3), to_swap);
    for (i = 0; i < 8; i++) {
        buf = read_uint8p(buf, &(guid->Data4[i]));
    }
    free(head);
    return err;
}
