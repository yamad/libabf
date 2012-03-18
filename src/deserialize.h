#ifndef DESERIALIZE_H
#define DESERIALIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* read_* functions
 * 
 * Each function reads the appropriate data type from the *buffer* at
 * the specified *offset*. For data sizes over one byte, if the *swap*
 * flag evaluates true, the returned value is byte-swapped. */
unsigned char read_char(const char *buffer, size_t offset);
uint8_t read_uint8(const char *buffer, size_t offset);
uint16_t read_uint16(const char *buffer, size_t offset, bool swap);
uint32_t read_uint32(const char *buffer, size_t offset, bool swap);
uint64_t read_uint64(const char *buffer, size_t offset, bool swap);
int8_t read_int8(const char *buffer, size_t offset);
int16_t read_int16(const char *buffer, size_t offset, bool swap);
int32_t read_int32(const char *buffer, size_t offset, bool swap);
int64_t read_int64(const char *buffer, size_t offset, bool swap);
float read_float32(const char *buffer, size_t offset, bool swap);
double read_float64(const char *buffer, size_t offset, bool swap);

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
char *read_charp(char *buf, char *to);
char *read_uint8p(char *buf, uint8_t *to);
char *read_int8p(char *buf, int8_t *to);
char *read_uint16p(char *buf, uint16_t *to, bool swap);
char *read_int16p(char *buf, int16_t *to, bool swap);
char *read_uint32p(char *buf, uint32_t *to, bool swap);
char *read_int32p(char *buf, int32_t *to, bool swap);
char *read_uint64p(char *buf, uint64_t *to, bool swap);
char *read_int64p(char *buf, int64_t *to, bool swap);

#endif
