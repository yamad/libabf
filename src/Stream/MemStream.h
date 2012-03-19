#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include <stdlib.h>

#include "Stream.h"

typedef struct MemStreamStruct * MemStream;

void *calloc_safe(int numberElements, size_t elementSize);
void *allocateByteArray(int numberBytes);

StreamError MemStream_Create(size_t numberBytes, Stream *stream);
StreamError MemStream_Destroy(Stream stream);

/* For testing only */
int8_t MemStream_getByteAt(Stream stream, streamPosition index);
StreamError MemStream_fillData(MemStream memstream, const uint8_t *from, size_t size);

#endif
