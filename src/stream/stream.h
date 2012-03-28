/**
 * @file stream.h
 * @brief generic data stream interface
 */
#ifndef STREAM_H
#define STREAM_H

#include "platform.h"
#include "swap.h"

/**
 * Base stream type
 *
 * This is the central type underlying the stream API, which can
 * handle generic streams of data. The stream API abstracts and
 * reimplements most functions acting on the FILE type in standard
 * C.
 *
 * This abstraction allows client code to work with streams from
 * arbitrary data sources. Functions that work on streams do not have
 * to know about where the actual data comes from. This feature is
 * particularly useful for testing purposes.
 *
 * The GNU libc extension fmemopen() is similar, but is
 * non-portable. In theory, the FILE type is already an abstraction
 * for streams. However, platform differences and the inaccessibility
 * of the type for client extension made FILE an impractical place to
 * hang one's hat.
 */
typedef struct stream stream_dt;


/**
 * A position in a stream
 *
 * This type can portably represent any position in a stream, and
 * should be used whenever a stream position is needed. The POSIX type
 * `off_t` has an identical purpose. On supported systems, streampos_dt
 * is a synonym for `off_t`.
 *
 * `fpos_t` is a standard type that serves a similar purpose, but is
 * only useful with fgetpos() and fsetpos() becasue it has an opaque,
 * system-dependent representation.
 */
typedef uint64_t streampos_dt;


/**
 * Return codes for the stream API
 *
 * Functions in the stream API will return one of these codes to
 * indicate error status.
 */
typedef enum STREAMERROR {
    STREAMERROR_UNKNOWN = -2,   /**< unknown/undefined outcome */
    STREAMERROR_FAILURE = -1,   /**< non-specific failure */
    STREAMERROR_SUCCESS = 0,    /**< non-specific success */
    STREAMERROR_NOMEMORY,       /**< insufficient memory */
    STREAMERROR_NOSPACE,        /**< insufficient space */
    STREAMERROR_NOOP            /**< no effect/operation. *not an error* */
} STREAMERROR;

STREAMERROR stream_create(size_t size, stream_dt *stream);
STREAMERROR stream_openForRead(const char *path, stream_dt *stream);
STREAMERROR stream_openForWrite(const char *path, stream_dt *stream);
STREAMERROR stream_close(stream_dt *stream);

STREAMERROR stream_read(stream_dt *stream, void *ptr, size_t size);
STREAMERROR stream_readn(stream_dt *stream, void *ptr, size_t size, size_t count);
STREAMERROR stream_write(stream_dt *stream, const void *ptr, size_t size);
STREAMERROR stream_writen(stream_dt *stream, const void *ptr, size_t size, size_t count);

STREAMERROR stream_seek(stream_dt *stream, streampos_dt offset, streampos_dt origin);
STREAMERROR stream_seekFromStart(stream_dt *stream, streampos_dt offset);
STREAMERROR stream_seekFromCurrent(stream_dt *stream, streampos_dt offset);
STREAMERROR stream_seekFromEnd(stream_dt *stream, streampos_dt offset);
STREAMERROR stream_seekToStart(stream_dt *stream);
STREAMERROR stream_seekToEnd(stream_dt *stream);

STREAMERROR stream_getCurrentPosition(stream_dt *stream, streampos_dt *curr_pos);
bool stream_isOpenForRead(stream_dt *stream);
bool stream_isOpenForWrite(stream_dt *stream);

bool stream_isPositionIn(stream_dt *stream, streampos_dt position);
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
