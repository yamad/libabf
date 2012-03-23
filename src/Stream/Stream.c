#include "Stream.h"

StreamError Stream_seekToStart(Stream stream)
{
    return Stream_seekFromStart(stream, 0);
}

StreamError Stream_seekToEnd(Stream stream)
{
    return Stream_seekFromEnd(stream, 0);
}

StreamError Stream_writeMultipleChunks(Stream stream, const void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return Stream_writeChunk(stream, ptr, totalSize);
}

StreamError Stream_readMultipleChunks(Stream stream, void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return Stream_readChunk(stream, ptr, totalSize);
}


StreamError Stream_write_uint8(Stream stream, const uint8_t from)
{
    return Stream_writeChunk(stream, &from, sizeof(uint8_t));
}

StreamError Stream_write_int8(Stream stream, const int8_t from)
{
    return Stream_writeChunk(stream, &from, sizeof(int8_t));
}

StreamError Stream_write_uint16(Stream stream, const uint16_t from, bool swap)
{
    uint16_t from_endian = swap ? _swap16(from) : from;       
    return Stream_writeChunk(stream, &from_endian, sizeof(uint16_t));
}

StreamError Stream_write_int16(Stream stream, const int16_t from, bool swap)
{
    return Stream_writeChunk(stream, &from, sizeof(int16_t));
}

StreamError Stream_read_uint8(Stream stream, uint8_t *to)
{
    return Stream_readChunk(stream, to, sizeof(uint8_t));
}

StreamError Stream_read_uint16(Stream stream, uint16_t *to, bool swap)
{
    StreamError err;
    err = Stream_readChunk(stream, to, sizeof(uint16_t));
    if (swap)
        *to = _swap16(*to);
    return err;
}
