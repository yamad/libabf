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
 * arbitrary data sources. Functions that work with streams do not
 * have to know any details about where the data or how it is
 * delivered. This feature is particularly useful for testing
 * purposes.
 *
 * The GNU libc extension fmemopen() is similar, but is
 * non-portable. In theory, the FILE type is already an abstraction
 * for streams. Unforunately, FILE is impractical to use for this case
 * because of platform differences and the inaccessibility of the type
 * for client extension.
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
typedef enum StreamError {
    StreamError_Unknown = -2,   /**< unknown/undefined outcome */
    StreamError_Failure = -1,   /**< non-specific failure */
    StreamError_Success = 0,    /**< non-specific success */
    StreamError_NoMemory,       /**< insufficient memory */
    StreamError_NoSpace,        /**< insufficient space */
    StreamError_NoOp            /**< no effect/operation. *not an error* */
} StreamError;


struct stream_operations {
    StreamError (*read)(stream_dt *stream, void *ptr, size_t size);
    StreamError (*write)(stream_dt *stream, const void *ptr, size_t size);
    StreamError (*seek)(stream_dt *stream, streampos_dt offset, streampos_dt origin);
    StreamError (*tell)(stream_dt *stream, streampos_dt *pos);
    bool (*posfits)(stream_dt *stream, streampos_dt pos); /**< checks if position fits in stream */
    bool (*sizefits)(stream_dt *stream, size_t size);     /**< checks if remaining space fits size bytes */
};

struct stream {
    struct stream_operations *ops;
    const char *type;
};

StreamError stream_create(size_t size, stream_dt *stream);
StreamError stream_openForRead(const char *path, stream_dt *stream);
StreamError stream_openForWrite(const char *path, stream_dt *stream);
StreamError stream_close(stream_dt *stream);

/**
 * Read `size` bytes from a stream into the memory area `ptr`.
 *
 * `ptr` must be large enough to hold `size`
 * bytes. StreamError_NoSpace is returned if the stream does not have
 * enough data remaining. The stream position is advanced by `size`.
 *
 * @see stream_write()
 */
StreamError stream_read(stream_dt *stream, void *ptr, size_t size);

/**
 * Read `n` elements of `size` bytes each from a stream into the
 * memory area `ptr`.
 *
 * `ptr` must be large enough to hold `size`*`n`
 * bytes. StreamError_NoSpace is returned if the stream does not have
 * enough data. The stream position is advanced by `size`*`n`.
 *
 * @see stream_read()
 */
StreamError stream_readn(stream_dt *stream, void *ptr, size_t size, size_t n);

/**
 * Write `size` bytes from memory `ptr` to stream `stream`.
 *
 * `ptr` must have at least `size` bytes of data. StreamError_NoSpace
 * is returned if the stream does not have sufficient space to hold
 * `size` bytes. The stream position is advanced by `size` bytes.
 *
 * @see stream_read(), stream_writen()
 **/
StreamError stream_write(stream_dt *stream, const void *ptr, size_t size);

/**
 * Write `n` elements of `size` bytes each from memory `ptr` to `stream`.
 *
 * `ptr` must have at least `size`*`n` bytes of
 * data. StreamError_NoSpace is returned if the stream does not have
 * sufficient space to hold `size`*`n` bytes. The stream position is
 * advanced by `size`*`n` bytes.
 *
 * @see stream_write(), stream_readn()
 **/
StreamError stream_writen(stream_dt *stream, const void *ptr, size_t size, size_t n);

StreamError stream_seek(stream_dt *stream, streampos_dt offset, streampos_dt origin);
StreamError stream_seekFromStart(stream_dt *stream, streampos_dt offset);
StreamError stream_seekFromCurrent(stream_dt *stream, streampos_dt offset);
StreamError stream_seekFromEnd(stream_dt *stream, streampos_dt offset);
StreamError stream_seekToStart(stream_dt *stream);
StreamError stream_seekToEnd(stream_dt *stream);

StreamError stream_tell(stream_dt *stream, streampos_dt *curr_pos);

bool stream_isOpenForRead(stream_dt *stream);
bool stream_isOpenForWrite(stream_dt *stream);

/** Return true if `position` fits within `stream` */
bool stream_posfits(stream_dt *stream, streampos_dt position);
/** Return true if `stream` has at least `size` bytes of remaining memory/data */
bool stream_sizefits(stream_dt *stream, size_t size);

StreamError stream_write_uint8(stream_dt *stream, const uint8_t from);
StreamError stream_write_int8(stream_dt *stream, const int8_t from);
StreamError stream_write_uint16(stream_dt *stream, const uint16_t from, bool swap);
StreamError stream_write_int16(stream_dt *stream, const int16_t from, bool swap);
StreamError stream_write_uint32(stream_dt *stream, const uint32_t from, bool swap);
StreamError stream_write_int32(stream_dt *stream, const int32_t from, bool swap);
StreamError stream_write_uint64(stream_dt *stream, const uint64_t from, bool swap);
StreamError stream_write_int64(stream_dt *stream, const int64_t from, bool swap);
StreamError stream_write_float(stream_dt *stream, const float from, bool swap);
StreamError stream_write_double(stream_dt *stream, const double from, bool swap);

StreamError stream_read_uint8(stream_dt *stream, uint8_t *to);
StreamError stream_read_int8(stream_dt *stream, int8_t *to);
StreamError stream_read_uint16(stream_dt *stream, uint16_t *to, bool swap);
StreamError stream_read_int16(stream_dt *stream, int16_t *to, bool swap);
StreamError stream_read_uint32(stream_dt *stream, uint32_t *to, bool swap);
StreamError stream_read_int32(stream_dt *stream, int32_t *to, bool swap);
StreamError stream_read_uint64(stream_dt *stream, uint64_t *to, bool swap);
StreamError stream_read_int64(stream_dt *stream, int64_t *to, bool swap);
StreamError stream_read_float(stream_dt *stream, float *to, bool swap);
StreamError stream_read_double(stream_dt *stream, double *to, bool swap);

#endif
