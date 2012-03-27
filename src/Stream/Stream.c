#include "Stream.h"

STREAMERROR stream_seekToStart(stream_dt *stream)
{
    return stream_seekFromStart(stream, 0);
}

STREAMERROR stream_seekToEnd(stream_dt *stream)
{
    return stream_seekFromEnd(stream, 0);
}

STREAMERROR stream_writen(stream_dt *stream, const void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return stream_write(stream, ptr, totalSize);
}

STREAMERROR stream_readn(stream_dt *stream, void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return stream_read(stream, ptr, totalSize);
}

STREAMERROR stream_write_uint8(stream_dt *stream, const uint8_t from)
{
    return stream_write(stream, &from, sizeof(uint8_t));
}

STREAMERROR stream_write_int8(stream_dt *stream, const int8_t from)
{
    return stream_write(stream, &from, sizeof(int8_t));
}

STREAMERROR stream_write_uint16(stream_dt *stream, const uint16_t from, bool swap)
{
    uint16_t from_endian = swap ? _swap16(from) : from;       
    return stream_write(stream, &from_endian, sizeof(uint16_t));
}

STREAMERROR stream_write_int16(stream_dt *stream, const int16_t from, bool swap)
{
    int16_t from_endian = swap ? _swap16(from) : from;
    return stream_write(stream, &from_endian, sizeof(int16_t));
}

STREAMERROR stream_write_uint32(stream_dt *stream, const uint32_t from, bool swap)
{
    uint32_t from_endian = swap ? _swap32(from) : from;       
    return stream_write(stream, &from_endian, sizeof(uint32_t));
}

STREAMERROR stream_write_int32(stream_dt *stream, const int32_t from, bool swap)
{
    int32_t from_endian = swap ? _swap32(from) : from;       
    return stream_write(stream, &from_endian, sizeof(int32_t));
}

STREAMERROR stream_write_uint64(stream_dt *stream, const uint64_t from, bool swap)
{
    uint64_t from_endian = swap ? _swap64(from) : from;       
    return stream_write(stream, &from_endian, sizeof(uint64_t));
}

STREAMERROR stream_write_int64(stream_dt *stream, const int64_t from, bool swap)
{
    int64_t from_endian = swap ? _swap64(from) : from;       
    return stream_write(stream, &from_endian, sizeof(int64_t));
}

STREAMERROR stream_read_uint8(stream_dt *stream, uint8_t *to)
{
    return stream_read(stream, to, sizeof(uint8_t));
}

STREAMERROR stream_read_uint16(stream_dt *stream, uint16_t *to, bool swap)
{
    STREAMERROR err;
    err = stream_read(stream, to, sizeof(uint16_t));
    if (swap)
        *to = _swap16(*to);
    return err;
}
