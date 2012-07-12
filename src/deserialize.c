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

uint16_t read_uint16(const char *buf, size_t offset, bool swap)
{
    uint16_t result;
    memcpy(&result, buf, sizeof(uint16_t));
    if (swap)
        result = _swap16(result);
    return result;
}

uint32_t read_uint32(const char *buf, size_t offset, bool swap)
{
    uint32_t result;
    memcpy(&result, buf, sizeof(uint32_t));
    if (swap)
        result = _swap32(result);
    return result;
}

uint64_t read_uint64(const char *buf, size_t offset, bool swap)
{
    uint64_t result;
    memcpy(&result, buf, sizeof(uint64_t));
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

char *read_charnp(char *buf, char *to, size_t nbytes)
{
    memcpy(to, buf, nbytes);
    return (buf + nbytes);
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
    return (buf + sizeof(*to));
}

char *read_int16p(char *buf, int16_t *to, bool swap)
{
    *to = read_int16(buf, 0, swap);
    return (buf + sizeof(*to));
}

char *read_uint32p(char *buf, uint32_t *to, bool swap)
{
    *to = read_uint32(buf, 0, swap);
    return (buf + sizeof(*to));
}

char *read_int32p(char *buf, int32_t *to, bool swap)
{
    *to = read_int32(buf, 0, swap);
    return (buf + sizeof(*to));
}

char *read_uint64p(char *buf, uint64_t *to, bool swap)
{
    *to = read_uint64(buf, 0, swap);
    return (buf + sizeof(*to));
}

char *read_int64p(char *buf, int64_t *to, bool swap)
{
    *to = read_int64(buf, 0, swap);
    return (buf + sizeof(*to));
}

char *read_float32p(char *buf, float *to, bool swap)
{
    *to = read_float32(buf, 0, swap);
    return (buf + sizeof(*to));
}

char *read_float64p(char *buf, double *to, bool swap)
{
    *to = read_float64(buf, 0, swap);
    return (buf + sizeof(*to));
}
