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

/* read_*p functions
 *
 * Each function reads the appropriate data type from the buffer
 * pointer *bufferp* and updates *bufferp* to the next unread
 * byte. For data sizes over one byte, if the *swap* evaluates true,
 * the returned value is byte-swapped.
 */
unsigned char read_charp(const char **bufferp);
uint8_t read_uint8p(const char **bufferp);
uint16_t read_uint16p(const char **bufferp, bool swap);
uint32_t read_uint32p(const char **bufferp, bool swap);
uint64_t read_uint64p(const char **bufferp, bool swap);
int8_t read_int8p(const char **bufferp);
int16_t read_int16p(const char **bufferp, bool swap);
int32_t read_int32p(const char **bufferp, bool swap);
int64_t read_int64p(const char **bufferp, bool swap);
float read_float32p(const char **bufferp, bool swap);
double read_float64p(const char **bufferp, bool swap);

#endif
