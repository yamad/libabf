#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include <stdlib.h>

#include "stream.h"

typedef struct memstream memstream_dt;

StreamError memstream_read(stream_dt *stream, void *ptr, size_t size);
StreamError memstream_write(stream_dt *stream, const void *ptr, size_t size);
StreamError memstream_seek(stream_dt *stream, streampos_dt offset, streampos_dt origin);
StreamError memstream_tell(stream_dt *stream, streampos_dt *pos);
bool memstream_posfits(stream_dt *stream, streampos_dt pos); /**< checks if position fits in stream */
bool memstream_sizefits(stream_dt *stream, size_t size);     /**< checks if remaining space fits size byte */

struct stream_operations memstream_ops = {
    .read = memstream_read,
    .write = memstream_write,
    .seek = memstream_seek,
    .tell = memstream_tell,
    .sizefits = memstream_sizefits,
    .posfits = memstream_posfits
};

void *calloc_safe(int numberElements, size_t elementSize);
void *allocateByteArray(int numberBytes);

StreamError memstream_create(size_t numberBytes, stream_dt **stream);
StreamError memstream_destroy(stream_dt *stream);

/* For testing only */
int8_t memstream_getByteAt(stream_dt *stream, streampos_dt index);
StreamError memstream_fillData(memstream_dt *memstream, const uint8_t *from, size_t size);

#endif
