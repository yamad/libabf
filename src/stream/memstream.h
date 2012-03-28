#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include <stdlib.h>

#include "stream.h"

typedef struct memstream memstream_dt;

void *calloc_safe(int numberElements, size_t elementSize);
void *allocateByteArray(int numberBytes);

StreamError memstream_create(size_t numberBytes, stream_dt **stream);
StreamError memstream_destroy(stream_dt *stream);

/* For testing only */
int8_t memstream_getByteAt(stream_dt *stream, streampos_dt index);
StreamError memstream_fillData(memstream_dt *memstream, const uint8_t *from, size_t size);

#endif
