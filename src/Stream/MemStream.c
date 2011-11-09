#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Stream.h"
#include "MemStream.h"

#include "memory.c"

typedef struct MemStreamStruct * MemStream;
typedef struct MemStreamStruct
{
    streamPosition data_length;
    streamPosition position;
    int8_t * data;
} MemStreamStruct;

Stream MemStream_Create(int numberBytes)
{
    MemStream self = NEW(MemStreamStruct);
    if (self == NULL) {
        return NULL;
    }
    self->data = allocateByteArray(numberBytes);
    if (self->data == NULL) {
        MemStream_Destroy((Stream)self);
        return NULL;
    }

    self->data_length = (size_t)numberBytes;
    self->position = 0;
    return (Stream)self;
}

void MemStream_Destroy(Stream super)
{
    MemStream self = (MemStream)super;
    FREE(self->data);
    FREE(self);
}

int8_t MemStream_getByteAt(Stream stream, streamPosition index)
{
    MemStream self = (MemStream) stream;
    if (isWithinStream((Stream)self, index)) {
        return self->data[index];
    }
    return FALSE;
}

void * allocateByteArray(int numberBytes)
{
    int8_t * ptr_mem = calloc_safe(numberBytes, sizeof(int8_t));
    if (ptr_mem == NULL) {
        return NULL;
    }
    return ptr_mem;
}

void *calloc_safe(int numberElements, size_t elementSize)
{
    void *mem;

    if (numberElements <= 0 || elementSize <= 0) {
        return NULL;
    }
    mem = calloc((size_t)numberElements, elementSize);
    return mem;
}

streamPosition Stream_getCurrentPosition(Stream stream)
{
    return ((MemStream)stream)->position;
}

int Stream_seek(Stream stream, streamPosition offset, streamPosition origin)
{
    MemStream self = (MemStream) stream;
    streamPosition new_position = offset + origin;
    if (isWithinStream((Stream)self, new_position)) {
        self->position = new_position;
    }
    return TRUE;
}

int Stream_seekFromStart(Stream stream, streamPosition offset)
{
    return Stream_seek(stream, offset, 0);
}

int Stream_seekFromCurrent(Stream stream, streamPosition offset)
{
    MemStream self = (MemStream) stream;
    return Stream_seek((Stream)self, offset, self->position);
}

int Stream_seekFromEnd(Stream stream, streamPosition offset)
{
    MemStream self = (MemStream) stream;
    streamPosition lastByte = self->data_length - 1;
    return Stream_seekFromStart((Stream)self, lastByte - offset);
}

int Stream_seekToStart(Stream stream)
{
    return Stream_seekFromStart(stream, 0);
}

int Stream_seekToEnd(Stream stream)
{
    return Stream_seekFromEnd(stream, 0);
}

int Stream_writeBlock(Stream stream, const void *ptr, size_t size)
{
    MemStream self = (MemStream) stream;
    if (!isEnoughSpace((Stream)self, size)) {
        return FALSE;
    }
    memcpy(self->data, ptr, size);
    Stream_seekFromCurrent((Stream)self, size);
    return TRUE;
}

int Stream_writeMultipleBlocks(Stream stream, const void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return Stream_writeBlock(stream, ptr, totalSize);
}

int Stream_readBlock(Stream stream, void *ptr, size_t size)
{
    MemStream self = (MemStream) stream;
    memcpy(ptr, self->data, size);
    Stream_seekFromCurrent((Stream)self, size);
    return TRUE;
}

int Stream_readMultipleBlocks(Stream stream, void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return Stream_readBlock(stream, ptr, totalSize);
}

Boolean isEnoughSpace(Stream stream, size_t size)
{
    MemStream self = (MemStream) stream;
    if (isWithinStream((Stream)self, self->position + size)) {
        return TRUE;
    }
    return FALSE;
}

Boolean isWithinStream(Stream stream, streamPosition position)
{
    MemStream self = (MemStream) stream;
    if (0 <= position && self->data_length > position) {
        return TRUE;
    }
    return FALSE;
}
