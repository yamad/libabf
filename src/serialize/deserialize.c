#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "deserialize.h"
#include "swap.h"

uint8_t read_uint8(const uint8_t *buf, size_t offset)
{
    return *(buf+offset);
}

uint16_t read_uint16(const uint8_t *buf, size_t offset, bool swap)
{
    uint16_t result;
    memcpy(&result, buf+offset, sizeof(result));
    if (swap)
        result = _swap16(result);
    return result;
}

uint32_t read_uint32(const uint8_t *buf, size_t offset, bool swap)
{
    uint32_t result;
    memcpy(&result, buf+offset, sizeof(result));
    if (swap)
        result = _swap32(result);
    return result;
}

uint64_t read_uint64(const uint8_t *buf, size_t offset, bool swap)
{
    uint64_t result;
    memcpy(&result, buf+offset, sizeof(result));
    if (swap)
        result = _swap64(result);
    return result;
}

int8_t read_int8(const uint8_t *buf, size_t offset)
{
    return (int8_t)read_uint8(buf, offset);
}

int16_t read_int16(const uint8_t *buf, size_t offset, bool swap)
{
    return (int16_t)read_uint16(buf, offset, swap);
}

int32_t read_int32(const uint8_t *buf, size_t offset, bool swap)
{
    return (int32_t)read_uint32(buf, offset, swap);
}

int64_t read_int64(const uint8_t *buf, size_t offset, bool swap)
{
    return (int64_t)read_uint64(buf, offset, swap);
}

float read_float32(const uint8_t *buf, size_t offset, bool swap)
{
    union
    {
        float f;
        uint32_t i;
    } u;

    u.i = read_uint32(buf, offset, swap);
    return u.f;
}

double read_float64(const uint8_t *buf, size_t offset, bool swap)
{
    union
    {
        double d;
        uint64_t i;
    } u;

    u.i = read_uint64(buf, offset, swap);
    return u.d;
}

char *read_string(const uint8_t* buf, size_t nmax, size_t *nconsumed) {
    size_t len = strlen((char*)buf);
    if (len > nmax)
        len = nmax;

    char *res = malloc(len+1);
    if (NULL == res)
        return NULL;

    strncpy(res, (char*)buf, len);
    *(res+len) = '\0';

    /* if null terminator is in buffer, count it as consumed */
    if (*(buf+len) == '\0')
        len++;
    *nconsumed = len;
    return res;
};

uint8_t *readn_uint8p(uint8_t *buf, uint8_t *to, size_t nbytes)
{
    memcpy(to, buf, nbytes);
    return (buf + nbytes);
}

uint8_t *read_uint8p(uint8_t *buf, uint8_t *to)
{
    *to = read_uint8(buf, 0);
    return (buf + sizeof(*to));
}

uint8_t *read_int8p(uint8_t *buf, int8_t *to)
{
    *to = read_int8(buf, 0);
    return (buf + sizeof(*to));
}

uint8_t *read_uint16p(uint8_t *buf, uint16_t *to, bool swap)
{
    *to = read_uint16(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_int16p(uint8_t *buf, int16_t *to, bool swap)
{
    *to = read_int16(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_uint32p(uint8_t *buf, uint32_t *to, bool swap)
{
    *to = read_uint32(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_int32p(uint8_t *buf, int32_t *to, bool swap)
{
    *to = read_int32(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_uint64p(uint8_t *buf, uint64_t *to, bool swap)
{
    *to = read_uint64(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_int64p(uint8_t *buf, int64_t *to, bool swap)
{
    *to = read_int64(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_float32p(uint8_t *buf, float *to, bool swap)
{
    *to = read_float32(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_float64p(uint8_t *buf, double *to, bool swap)
{
    *to = read_float64(buf, 0, swap);
    return (buf + sizeof(*to));
}

uint8_t *read_stringp(uint8_t* buf, char **to, size_t nmax) {
    size_t nconsumed;
    *to = read_string(buf, nmax, &nconsumed);
    return (buf+nconsumed);         /* advance past terminator */
};
