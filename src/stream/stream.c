#include "stream.h"

StreamError stream_read(stream_dt *stream, void *ptr, size_t size)
{
    return stream->ops->read(stream, ptr, size);
}

StreamError stream_write(stream_dt *stream, const void *ptr, size_t size)
{
    return stream->ops->write(stream, ptr, size);
}

StreamError stream_seek(stream_dt *stream, streampos_dt offset, streampos_dt origin)
{
    return stream->ops->seek(stream, offset, origin);
}

StreamError stream_tell(stream_dt *stream, streampos_dt *curr_pos)
{
    return stream->ops->tell(stream, curr_pos);
}

StreamError stream_seekToStart(stream_dt *stream)
{
    return stream_seekFromStart(stream, 0);
}

StreamError stream_seekToEnd(stream_dt *stream)
{
    return stream_seekFromEnd(stream, 0);
}

StreamError stream_seekFromStart(stream_dt *stream, streampos_dt offset)
{
    return stream_seek(stream, offset, 0);
}

StreamError stream_writen(stream_dt *stream, const void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return stream_write(stream, ptr, totalSize);
}

StreamError stream_readn(stream_dt *stream, void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return stream_read(stream, ptr, totalSize);
}


StreamError stream_write_uint8(stream_dt *stream, const uint8_t from)
{
    return stream_write(stream, &from, sizeof(from));
}

StreamError stream_write_int8(stream_dt *stream, const int8_t from)
{
    return stream_write(stream, &from, sizeof(from));
}

StreamError stream_write_uint16(stream_dt *stream, const uint16_t from, bool swap)
{
    uint16_t from_endian = swap ? _swap16(from) : from;
    return stream_write(stream, &from_endian, sizeof(from));
}

StreamError stream_write_int16(stream_dt *stream, const int16_t from, bool swap)
{
    int16_t from_endian = swap ? _swap16(from) : from;
    return stream_write(stream, &from_endian, sizeof(from));
}

StreamError stream_write_uint32(stream_dt *stream, const uint32_t from, bool swap)
{
    uint32_t from_endian = swap ? _swap32(from) : from;
    return stream_write(stream, &from_endian, sizeof(from));
}

StreamError stream_write_int32(stream_dt *stream, const int32_t from, bool swap)
{
    int32_t from_endian = swap ? _swap32(from) : from;
    return stream_write(stream, &from_endian, sizeof(from));
}

StreamError stream_write_uint64(stream_dt *stream, const uint64_t from, bool swap)
{
    uint64_t from_endian = swap ? _swap64(from) : from;
    return stream_write(stream, &from_endian, sizeof(from));
}

StreamError stream_write_int64(stream_dt *stream, const int64_t from, bool swap)
{
    int64_t from_endian = swap ? _swap64(from) : from;
    return stream_write(stream, &from_endian, sizeof(from));
}


StreamError stream_read_uint8(stream_dt *stream, uint8_t *to)
{
    return stream_read(stream, to, sizeof(*to));
}

StreamError stream_read_int8(stream_dt *stream, int8_t *to)
{
    return stream_read(stream, to, sizeof(*to));
}

StreamError stream_read_uint16(stream_dt *stream, uint16_t *to, bool swap)
{
    StreamError err;
    err = stream_read(stream, to, sizeof(*to));
    if (swap)
        *to = _swap16(*to);
    return err;
}

StreamError stream_read_int16(stream_dt *stream, int16_t *to, bool swap)
{
    StreamError err;
    err = stream_read(stream, to, sizeof(*to));
    if (swap)
        *to = _swap16(*to);
    return err;
}

StreamError stream_read_uint32(stream_dt *stream, uint32_t *to, bool swap)
{
    StreamError err;
    err = stream_read(stream, to, sizeof(*to));
    if (swap)
        *to = _swap32(*to);
    return err;
}

StreamError stream_read_int32(stream_dt *stream, int32_t *to, bool swap)
{
    StreamError err;
    err = stream_read(stream, to, sizeof(*to));
    if (swap)
        *to = _swap32(*to);
    return err;
}

StreamError stream_read_uint64(stream_dt *stream, uint64_t *to, bool swap)
{
    StreamError err;
    err = stream_read(stream, to, sizeof(*to));
    if (swap)
        *to = _swap64(*to);
    return err;
}

StreamError stream_read_int64(stream_dt *stream, int64_t *to, bool swap)
{
    StreamError err;
    err = stream_read(stream, to, sizeof(*to));
    if (swap)
        *to = _swap64(*to);
    return err;
}
