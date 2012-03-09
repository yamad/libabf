#include "Stream.h"

StreamError Stream_write_uint8(Stream stream, const uint8_t *in)
{
    return Stream_writeChunk(stream, in, sizeof(uint8_t));
}

StreamError Stream_write_int8(Stream stream, const int8_t *in)
{
    return Stream_writeChunk(stream, in, sizeof(int8_t));
}

StreamError Stream_read_uint8(Stream stream, uint8_t *out)
{
    return Stream_readChunk(stream, out, sizeof(uint8_t));
}
