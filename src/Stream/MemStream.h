#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include <stdlib.h>

#include "Stream.h"

void *calloc_safe(int numberElements, size_t elementSize);
void *allocateByteArray(int numberBytes);

StreamError MemStream_Create(size_t numberBytes, Stream *stream);
StreamError MemStream_Destroy(Stream stream);

/* For testing only */
int8_t MemStream_getByteAt(Stream stream, streamPosition index);

#endif
