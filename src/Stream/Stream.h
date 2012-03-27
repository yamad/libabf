#ifndef STREAM_H
#define STREAM_H

#include "platform.h"
#include "swap.h"

typedef struct stream stream_dt;

typedef off_t streamPosition;

/* Stream error states */
typedef enum STREAMERROR {
    STREAMERROR_UNKNOWN = -2,   /* unknown/undefined outcome */
    STREAMERROR_FAILURE = -1,   /* non-specific failure */
    STREAMERROR_SUCCESS = 0,    /* non-specific success */
    STREAMERROR_NOMEMORY,       /* insufficient memory */
    STREAMERROR_NOSPACE,        /* insufficient space */
    STREAMERROR_NOOP            /* no effect/operation. *not an error* */
} STREAMERROR;

STREAMERROR stream_create(size_t size, stream_dt *stream);
STREAMERROR stream_openForRead(const char *path, stream_dt *stream);
STREAMERROR stream_openForWrite(const char *path, stream_dt *stream);
STREAMERROR stream_close(stream_dt *stream);

STREAMERROR stream_read(stream_dt *stream, void *ptr, size_t size);
STREAMERROR stream_readn(stream_dt *stream, void *ptr, size_t size, size_t count);
STREAMERROR stream_write(stream_dt *stream, const void *ptr, size_t size);
STREAMERROR stream_writen(stream_dt *stream, const void *ptr, size_t size, size_t count);

STREAMERROR stream_seek(stream_dt *stream, streamPosition offset, streamPosition origin);
STREAMERROR stream_seekFromStart(stream_dt *stream, streamPosition offset);
STREAMERROR stream_seekFromCurrent(stream_dt *stream, streamPosition offset);
STREAMERROR stream_seekFromEnd(stream_dt *stream, streamPosition offset);
STREAMERROR stream_seekToStart(stream_dt *stream);
STREAMERROR stream_seekToEnd(stream_dt *stream);

STREAMERROR stream_getCurrentPosition(stream_dt *stream, streamPosition *curr_pos);
bool stream_isOpenForRead(stream_dt *stream);
bool stream_isOpenForWrite(stream_dt *stream);

bool stream_isPositionIn(stream_dt *stream, streamPosition position);
bool stream_hasSpace(stream_dt *stream, size_t size);

STREAMERROR stream_write_uint8(stream_dt *stream, const uint8_t from);
STREAMERROR stream_write_int8(stream_dt *stream, const int8_t from);
STREAMERROR stream_write_uint16(stream_dt *stream, const uint16_t from, bool swap);
STREAMERROR stream_write_int16(stream_dt *stream, const int16_t from, bool swap);
STREAMERROR stream_write_uint32(stream_dt *stream, const uint32_t from, bool swap);
STREAMERROR stream_write_int32(stream_dt *stream, const int32_t from, bool swap);
STREAMERROR stream_write_uint64(stream_dt *stream, const uint64_t from, bool swap);
STREAMERROR stream_write_int64(stream_dt *stream, const int64_t from, bool swap);
STREAMERROR stream_write_float(stream_dt *stream, const float from, bool swap);
STREAMERROR stream_write_double(stream_dt *stream, const double from, bool swap);

STREAMERROR stream_read_uint8(stream_dt *stream, uint8_t *to);
STREAMERROR stream_read_int8(stream_dt *stream, int8_t *to);
STREAMERROR stream_read_uint16(stream_dt *stream, uint16_t *to, bool swap);
STREAMERROR stream_read_int16(stream_dt *stream, int16_t *to, bool swap);
STREAMERROR stream_read_uint32(stream_dt *stream, uint32_t *to, bool swap);
STREAMERROR stream_read_int32(stream_dt *stream, int32_t *to, bool swap);
STREAMERROR stream_read_uint64(stream_dt *stream, uint64_t *to, bool swap);
STREAMERROR stream_read_int64(stream_dt *stream, int64_t *to, bool swap);
STREAMERROR stream_read_float(stream_dt *stream, float *to, bool swap);
STREAMERROR stream_read_double(stream_dt *stream, double *to, bool swap);

#endif
