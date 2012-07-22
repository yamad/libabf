#ifndef LIBABF_SWAP_H_
#define LIBABF_SWAP_H_

#include <stdint.h>

typedef enum byte_order {
    ENDIAN_UNKNOWN = -1,
    ENDIAN_LITTLE,
    ENDIAN_BIG
} byte_order;

int get_endian(void);

/* Return a byteswapped version of a 16-bit value */
#define _swap16(x) ((uint16_t)(                  \
    (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

/* Return a byteswapped version of a 32-bit value */
#define _swap32(x) ((uint32_t)(                        \
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

/* Return a byteswapped version of a 64-bit value */
#define _swap64(x) ((uint64_t)(                                 \
    (((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
    (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
    (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
    (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
    (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
    (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
    (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
    (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)))

#endif /* LIBABF_SWAP_H_ */
