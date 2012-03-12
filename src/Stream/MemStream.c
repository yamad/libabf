#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Stream.h"
#include "MemStream.h"

#include "memory.c"

typedef struct MemStreamStruct
{
    streamPosition data_length;
    streamPosition position;
    int8_t * data;
} MemStreamStruct;

StreamError MemStream_Create(size_t numberBytes, Stream *stream)
{
    *stream = NULL;
    MemStream self = NEW(MemStreamStruct);
    if (self == NULL) {
        return StreamError_Failure;
    }
    self->data = allocateByteArray(numberBytes);
    if (self->data == NULL) {
        MemStream_Destroy((Stream)self);
        return StreamError_NoMemory;
    }

    self->data_length = (size_t)numberBytes;
    self->position = 0;
    *stream = (Stream)self;
    return StreamError_Success;
}

StreamError MemStream_Destroy(Stream super)
{
    MemStream self = (MemStream)super;
    FREE(self->data);
    FREE(self);
    return StreamError_Success;
}

int8_t MemStream_getByteAt(Stream stream, streamPosition index)
{
    MemStream self = (MemStream) stream;
    if (Stream_isPositionIn((Stream)self, index)) {
        return self->data[index];
    }
    return FALSE;
}

void *allocateByteArray(int numberBytes)
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

StreamError Stream_getCurrentPosition(Stream stream, streamPosition *curr_pos)
{
    *curr_pos = ((MemStream)stream)->position;
    return StreamError_Success;
}

StreamError Stream_seek(Stream stream, streamPosition offset, streamPosition origin)
{
    MemStream self = (MemStream) stream;
    streamPosition new_position = offset + origin;
    if (Stream_isPositionIn((Stream)self, new_position)) {
        self->position = new_position;
    }
    return StreamError_Success;
}

StreamError Stream_seekFromStart(Stream stream, streamPosition offset)
{
    return Stream_seek(stream, offset, 0);
}

StreamError Stream_seekFromCurrent(Stream stream, streamPosition offset)
{
    MemStream self = (MemStream) stream;
    return Stream_seek((Stream)self, offset, self->position);
}

StreamError Stream_seekFromEnd(Stream stream, streamPosition offset)
{
    MemStream self = (MemStream) stream;
    streamPosition lastByte = self->data_length - 1;
    return Stream_seekFromStart((Stream)self, lastByte - offset);
}

StreamError Stream_writeChunk(Stream stream, const void *ptr, size_t size)
{
    MemStream self = (MemStream) stream;
    if (!Stream_hasSpace((Stream)self, size)) {
        return StreamError_NoSpace;
    }
    memcpy(self->data, ptr, size);
    Stream_seekFromCurrent((Stream)self, size);
    return StreamError_Success;
}

StreamError Stream_readChunk(Stream stream, void *ptr, size_t size)
{
    MemStream self = (MemStream) stream;
    memcpy(ptr, self->data, size);
    Stream_seekFromCurrent((Stream)self, size);
    return StreamError_Success;
}

Boolean Stream_hasSpace(Stream stream, size_t size)
{
    MemStream self = (MemStream) stream;
    if (Stream_isPositionIn((Stream)self, self->position + size)) {
        return TRUE;
    }
    return FALSE;
}

Boolean Stream_isPositionIn(Stream stream, streamPosition position)
{
    MemStream self = (MemStream) stream;
    if (0 <= position && self->data_length > position) {
        return TRUE;
    }
    return FALSE;
}

StreamError MemStream_fillData(MemStream memstream, const uint8_t *from, size_t size)
{
    if (!Stream_hasSpace((Stream)memstream, size))
        return StreamError_NoSpace;
    memcpy(memstream->data, from, size);
    return StreamError_Success;
}
