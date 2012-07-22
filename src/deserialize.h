#ifndef DESERIALIZE_H
#define DESERIALIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* read_* functions
 *
 * Each function reads the appropriate data type from buffer *buf* at
 * the specified *offset*. For data sizes over one byte, if the *swap*
 * flag evaluates true, the returned value is byte-swapped. */
uint8_t read_uint8(const uint8_t *buf, size_t offset);
uint16_t read_uint16(const uint8_t *buf, size_t offset, bool swap);
uint32_t read_uint32(const uint8_t *buf, size_t offset, bool swap);
uint64_t read_uint64(const uint8_t *buf, size_t offset, bool swap);
int8_t read_int8(const uint8_t *buf, size_t offset);
int16_t read_int16(const uint8_t *buf, size_t offset, bool swap);
int32_t read_int32(const uint8_t *buf, size_t offset, bool swap);
int64_t read_int64(const uint8_t *buf, size_t offset, bool swap);
float read_float32(const uint8_t *buf, size_t offset, bool swap);
double read_float64(const uint8_t *buf, size_t offset, bool swap);

/** read_*p functions
 *
 *  Reads a value of the appropriate data type from a byte buffer into
 *  a destination variable. If *swap* is true, for data sizes over one
 *  byte, the value is byte-swapped. Returns an updated pointer to the
 *  next unread byte in the byte buffer.
 *
 *  Note that it is possible to read off the end of the buffer, and
 *  this condition must be checked.
 */
uint8_t *read_uint8p(uint8_t *buf, uint8_t *to);
uint8_t *readn_uint8p(uint8_t *buf, uint8_t *to, size_t nbytes);
uint8_t *read_int8p(uint8_t *buf, int8_t *to);
uint8_t *read_uint16p(uint8_t *buf, uint16_t *to, bool swap);
uint8_t *read_int16p(uint8_t *buf, int16_t *to, bool swap);
uint8_t *read_uint32p(uint8_t *buf, uint32_t *to, bool swap);
uint8_t *read_int32p(uint8_t *buf, int32_t *to, bool swap);
uint8_t *read_uint64p(uint8_t *buf, uint64_t *to, bool swap);
uint8_t *read_int64p(uint8_t *buf, int64_t *to, bool swap);
uint8_t *read_float32p(uint8_t *buf, float *to, bool swap);
uint8_t *read_float64p(uint8_t *buf, double *to, bool swap);

#endif
