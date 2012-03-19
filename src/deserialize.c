#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "deserialize.h"
#include "swap.h"

unsigned char read_char(const char *buffer, size_t offset)
{
    assert(NULL != buffer);
    return *(buffer+offset);
}

uint8_t read_uint8(const char *buffer, size_t offset)
{
    assert(CHAR_BIT == 8);
    return (uint8_t)read_char(buffer, offset);
}

uint16_t read_uint16(const char *buffer, size_t offset, bool swap)
{
    uint8_t b1 = read_uint8(buffer, offset);
    uint8_t b2 = read_uint8(buffer, offset+1);
    uint16_t result = \
        ((uint16_t)(                            \
            ((uint16_t)b1 << 8) |               \
            ((uint16_t)b2 << 0)));
    if (swap)
        result = _swap16(result);
    return result;
}

uint32_t read_uint32(const char *buffer, size_t offset, bool swap)
{
    uint8_t b1 = read_uint8(buffer, offset);
    uint8_t b2 = read_uint8(buffer, offset+1);
    uint8_t b3 = read_uint8(buffer, offset+2);
    uint8_t b4 = read_uint8(buffer, offset+3);
    uint32_t result = \
        ((uint32_t)(                            \
            ((uint32_t)b1 << 24) |              \
            ((uint32_t)b2 << 16) |              \
            ((uint32_t)b3 <<  8) |              \
            ((uint32_t)b4 <<  0)));
    if (swap)
        result = _swap32(result);
    return result;
}

uint32_t read_uint32_2(const char *buffer, size_t offset, bool swap)
{
    uint32_t result;
    memcpy(&result, (buffer+offset), sizeof(uint32_t));
    if (swap)
        result = _swap32(result);
    return result;
}
 
uint64_t read_uint64(const char *buffer, size_t offset, bool swap)
{
    uint8_t b1 = read_uint8(buffer, offset);
    uint8_t b2 = read_uint8(buffer, offset+1);
    uint8_t b3 = read_uint8(buffer, offset+2);
    uint8_t b4 = read_uint8(buffer, offset+3);
    uint8_t b5 = read_uint8(buffer, offset+4);
    uint8_t b6 = read_uint8(buffer, offset+5);
    uint8_t b7 = read_uint8(buffer, offset+6);
    uint8_t b8 = read_uint8(buffer, offset+7);
    uint64_t result = \
        ((uint64_t)(                                                    \
            ((uint64_t)b1 << 56) |                                      \
            ((uint64_t)b2 << 48) |                                      \
            ((uint64_t)b3 << 40) |                                      \
            ((uint64_t)b4 << 32) |                                      \
            ((uint64_t)b5 << 24) |                                      \
            ((uint64_t)b6 << 16) |                                      \
            ((uint64_t)b7 <<  8) |                                      \
            ((uint64_t)b8 <<  0)));
    if (swap)
        result = _swap64(result);
    return result;
}

int8_t read_int8(const char *buffer, size_t offset)
{
    return (int8_t)read_uint8(buffer, offset);
}

int16_t read_int16(const char *buffer, size_t offset, bool swap)
{
    return (int16_t)read_uint16(buffer, offset, swap);
}

int32_t read_int32(const char *buffer, size_t offset, bool swap)
{
    return (int32_t)read_uint32(buffer, offset, swap);
}

int64_t read_int64(const char *buffer, size_t offset, bool swap)
{
    return (int64_t)read_uint64(buffer, offset, swap);
}

float read_float32(const char *buffer, size_t offset, bool swap)
{
    union
    {
        float f;
        uint32_t i;
    } u;

    u.i = read_uint32(buffer, offset, swap);
    return u.f;
}

double read_float64(const char *buffer, size_t offset, bool swap)
{
    union
    {
        double d;
        uint64_t i;
    } u;

    u.i = read_uint64(buffer, offset, swap);
    return u.d;
}

char *read_charp(char *buf, char *to)
{
    *to = read_char(buf, 0);
    return ++buf;
}

char *read_uint8p(char *buf, uint8_t *to)
{
    *to = read_uint8(buf, 0);
    return ++buf;
}

char *read_int8p(char *buf, int8_t *to)
{
    *to = read_int8(buf, 0);
    return ++buf;
}

char *read_uint16p(char *buf, uint16_t *to, bool swap)
{
    *to = read_uint16(buf, 0, swap);
    return (buf + sizeof(uint16_t));
}

char *read_int16p(char *buf, int16_t *to, bool swap)
{
    *to = read_int16(buf, 0, swap);
    return (buf + sizeof(int16_t));
}

char *read_uint32p(char *buf, uint32_t *to, bool swap)
{
    *to = read_uint32(buf, 0, swap);
    return (buf + sizeof(uint32_t));
}

char *read_int32p(char *buf, int32_t *to, bool swap)
{
    *to = read_int32(buf, 0, swap);
    return (buf + sizeof(int32_t));
}

char *read_uint64p(char *buf, uint64_t *to, bool swap)
{
    *to = read_uint64(buf, 0, swap);
    return (buf + sizeof(uint64_t));
}

char *read_int64p(char *buf, int64_t *to, bool swap)
{
    *to = read_int64(buf, 0, swap);
    return (buf + sizeof(int64_t));
}
