#ifndef STREAM_H
#define STREAM_H

#include "platform.h"

typedef struct StreamStruct * Stream;

typedef off_t streamPosition;

/* Stream error states */
typedef enum {
    StreamError_Failure = 0,
    StreamError_Success,
    StreamError_NoMemory,
} StreamError;

StreamError Stream_Create(size_t size, Stream *stream);
Stream Stream_OpenForRead(const char *path);
Stream Stream_OpenForWrite(const char *path);
int Stream_Close(Stream stream);

int Stream_readChunk(Stream stream, void *ptr, size_t size);
int Stream_readMultipleChunks(Stream stream, void *ptr, size_t size, size_t count);
int Stream_writeChunk(Stream stream, const void *ptr, size_t size);
int Stream_writeMultipleChunks(Stream stream, const void *ptr, size_t size, size_t count);

StreamError Stream_seek(Stream stream, streamPosition offset, streamPosition origin);
int Stream_seekFromStart(Stream stream, streamPosition offset);
int Stream_seekFromCurrent(Stream stream, streamPosition offset);
int Stream_seekFromEnd(Stream stream, streamPosition offset);
int Stream_seekToStart(Stream stream);
int Stream_seekToEnd(Stream stream);

StreamError Stream_getCurrentPosition(Stream stream, streamPosition *curr_pos);
Boolean Stream_isOpenForRead(Stream stream);
Boolean Stream_isOpenForWrite(Stream stream);

#endif
