#ifndef LIBABF_SWAP_H_
#define LIBABF_SWAP_H_

#include <stdint.h>

/* Return a byteswapped version of a 16-bit value */
/* uint16_t swapBytes16(uint16_t); */

#define _swap16(to_swap) ((uint16_t)(                   \
    (((uint16_t)(to_swap) & (uint16_t)0x00ffU) << 8) |  \
    (((uint16_t)(to_swap) & (uint16_t)0xff00U) >> 8)))

#endif /* LIBABF_SWAP_H_ */
