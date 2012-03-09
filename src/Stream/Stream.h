#ifndef STREAM_H
#define STREAM_H

#include "platform.h"

typedef struct StreamStruct * Stream;

struct stream;

typedef off_t streamPosition;

/* Stream error states */
typedef enum {
    StreamError_Unknown = -2,
    StreamError_Failure = -1,
    StreamError_Success = 0,
    StreamError_NoMemory,
    StreamError_NoSpace,
} StreamError;

StreamError Stream_Create(size_t size, Stream *stream);
StreamError Stream_OpenForRead(const char *path, Stream *stream);
StreamError Stream_OpenForWrite(const char *path, Stream *stream);
StreamError Stream_Close(Stream stream);

StreamError Stream_readChunk(Stream stream, void *ptr, size_t size);
StreamError Stream_readMultipleChunks(Stream stream, void *ptr, size_t size, size_t count);
StreamError Stream_writeChunk(Stream stream, const void *ptr, size_t size);
StreamError Stream_Writemultiplechunks(Stream stream, const void *ptr, size_t size, size_t count);

StreamError Stream_seek(Stream stream, streamPosition offset, streamPosition origin);
StreamError Stream_seekFromStart(Stream stream, streamPosition offset);
StreamError Stream_seekFromCurrent(Stream stream, streamPosition offset);
StreamError Stream_seekFromEnd(Stream stream, streamPosition offset);
StreamError Stream_seekToStart(Stream stream);
StreamError Stream_seekToEnd(Stream stream);

StreamError Stream_getCurrentPosition(Stream stream, streamPosition *curr_pos);
Boolean Stream_isOpenForRead(Stream stream);
Boolean Stream_isOpenForWrite(Stream stream);

Boolean Stream_isPositionIn(Stream stream, streamPosition position);
Boolean Stream_hasSpace(Stream stream, size_t size);

StreamError Stream_write_uint8(Stream stream, const uint8_t *in);
StreamError Stream_write_int8(Stream stream, const int8_t *in);
StreamError Stream_write_uint16(Stream stream, const uint16_t *in);
StreamError Stream_write_int16(Stream stream, const int16_t *in);
StreamError Stream_write_uint32(Stream stream, const uint32_t *in);
StreamError Stream_write_int32(Stream stream, const int32_t *in);
StreamError Stream_write_uint64(Stream stream, const uint64_t *in);
StreamError Stream_write_int64(Stream stream, const int64_t *in);

StreamError Stream_read_uint8(Stream stream, uint8_t *out);

#endif
