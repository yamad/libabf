#include "swap.h"

/* get_endian -- test the byte order at runtime and return the
 * appropriate value from the enum `byte_order`
 *
 * approach is taken from: C: A Reference Manual, 5th ed. (Harbison
 * and Steele), sec. 6.1.2
 */
int get_endian(void)
{
    static union {
        long as_long;
        char as_char[sizeof(long)];
    } u;
    u.as_long = 1;
    if (u.as_char[0] == 1)
        return ENDIAN_LITTLE;
    else if (u.as_char[sizeof(long)-1] == 1)
        return ENDIAN_BIG;
    return ENDIAN_UNKNOWN;
}
