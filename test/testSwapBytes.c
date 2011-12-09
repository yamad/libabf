#include "unity.h"
#include "swap.h"

#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}

union twobytes {
    uint16_t as_int;
    char as_char[2];
};

void test_swap16_should_swap_2bytes(void)
{
    union twobytes unswapped;
    unswapped.as_char[0] = 0xCA;
    unswapped.as_char[1] = 0xFE;
    union twobytes swapped;

    swapped.as_int = _swap16(unswapped.as_int);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[0], swapped.as_char[1]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[1], swapped.as_char[0]);
}

