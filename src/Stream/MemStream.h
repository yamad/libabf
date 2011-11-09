#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include <stdlib.h>

#include "Stream.h"

void *calloc_safe(int numberElements, size_t elementSize);
void * allocateByteArray(int numberBytes);

Stream MemStream_Create(int numberBytes);
void MemStream_Destroy(Stream stream);

int8_t MemStream_getByteAt(Stream stream, streamPosition index);

Boolean isWithinStream(Stream, streamPosition);
Boolean isEnoughSpace(Stream, size_t);

#endif
