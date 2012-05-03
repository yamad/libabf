#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"
#include "memstream.h"

#include "memory.c"

struct memstream
{
    stream_dt base;
    streampos_dt data_length;
    streampos_dt position;
    uint8_t *data;
};

struct stream_operations memstream_ops = {
    .read = memstream_read,
    .write = memstream_write,
    .seek = memstream_seek,
    .tell = memstream_tell,
    .sizefits = memstream_sizefits,
    .posfits = memstream_posfits
};

StreamError memstream_create(size_t numberBytes, stream_dt **stream)
{
    *stream = NULL;
    memstream_dt *self = NEW(memstream_dt);
    if (self == NULL) {
        return StreamError_Failure;
    }

    self->base.type = "MemStream";
    self->base.ops = &memstream_ops;

    self->data = allocateByteArray(numberBytes);
    if (self->data == NULL) {
        memstream_destroy((stream_dt*)self);
        return StreamError_NoMemory;
    }

    self->data_length = (size_t)numberBytes;
    self->position = 0;
    *stream = (stream_dt*)self;
    return StreamError_Success;
}

StreamError memstream_destroy(stream_dt *super)
{
    memstream_dt* self = (memstream_dt*)super;
    FREE(self->data);
    FREE(self);
    return StreamError_Success;
}

int8_t memstream_getByteAt(stream_dt *stm, streampos_dt index)
{
    memstream_dt* self = (memstream_dt*) stm;
    if (memstream_posfits((stream_dt*)self, index)) {
        return self->data[index];
    }
    return false;
}

void *allocateByteArray(int numberBytes)
{
    int8_t *ptr_mem = calloc_safe(numberBytes, sizeof(uint8_t));
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

StreamError memstream_tell(stream_dt *stream, streampos_dt *curr_pos)
{
    *curr_pos = ((memstream_dt*)stream)->position;
    return StreamError_Success;
}

StreamError memstream_seek(stream_dt *stream, streampos_dt offset, streampos_dt origin)
{
    memstream_dt* self = (memstream_dt*)stream;
    streampos_dt new_position = offset + origin;
    if (memstream_posfits((stream_dt*)self, new_position)) {
        self->position = new_position;
    }
    return StreamError_Success;
}

StreamError stream_seekFromEnd(stream_dt *stm, streampos_dt offset)
{
    memstream_dt *self = (memstream_dt*) stm;
    streampos_dt lastByte = self->data_length - 1;
    return memstream_seek((stream_dt*)self, -offset, lastByte);
}

StreamError memstream_write(stream_dt *stream, const void *ptr, size_t size)
{
    StreamError err;
    memstream_dt* self = (memstream_dt*) stream;
    if (!memstream_sizefits((stream_dt*)self, size)) {
        return StreamError_NoSpace;
    }
    memcpy(self->data + self->position, ptr, size);
    err = memstream_seek((stream_dt*)self, size, self->position);
    if (StreamError_Success != err)
        return err;
    return StreamError_Success;
}

StreamError memstream_read(stream_dt *stream, void *ptr, size_t size)
{
    StreamError err;
    memstream_dt *self = (memstream_dt*)stream;
    if (!memstream_sizefits((stream_dt*)self, size)) {
        return StreamError_NoSpace;
    }
    memcpy(ptr, self->data + self->position, size);
    err = memstream_seek((stream_dt*)self, size, self->position);
    if (StreamError_Success != err)
        return err;
    return StreamError_Success;
}

bool memstream_posfits(stream_dt *stream, streampos_dt position)
{
    memstream_dt *self = (memstream_dt*) stream;
    if (0 <= position && self->data_length > position) {
        return true;
    }
    return false;
}

bool memstream_sizefits(stream_dt *stream, size_t size)
{
    memstream_dt *self = (memstream_dt*) stream;
    if (memstream_posfits((stream_dt*)self, self->position + size)) {
        return true;
    }
    return false;
}

StreamError memstream_fillData(memstream_dt *ms, const uint8_t *from, size_t size)
{
    if (!memstream_sizefits((stream_dt *)ms, size))
        return StreamError_NoSpace;
    memcpy(ms->data, from, size);
    return StreamError_Success;
}
