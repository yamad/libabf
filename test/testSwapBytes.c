#include "unity.h"
#include "swap.h"

#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}

union twobytes
{
    uint16_t as_int;
    char as_char[2];
};

union fourbytes 
{
    uint32_t as_int;
    char as_char[4];
};

union eightbytes
{
    uint64_t as_int;
    char as_char[8];
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

void test_swap32_should_swap_4bytes(void)
{
    union fourbytes unswapped;
    unswapped.as_char[0] = 0xBE;
    unswapped.as_char[1] = 0xEF;
    unswapped.as_char[2] = 0xCA;
    unswapped.as_char[3] = 0xFE;

    union fourbytes swapped;
    swapped.as_int = _swap32(unswapped.as_int);

    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[0], swapped.as_char[3]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[1], swapped.as_char[2]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[2], swapped.as_char[1]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[3], swapped.as_char[0]);    
}

void test_swap64_should_swap_8bytes(void)
{
    union eightbytes unswapped;
    unswapped.as_char[0] = 0xBE;
    unswapped.as_char[1] = 0xEF;
    unswapped.as_char[2] = 0xCA;
    unswapped.as_char[3] = 0xFE;
    unswapped.as_char[4] = 0x12;
    unswapped.as_char[5] = 0x34;
    unswapped.as_char[6] = 0x56;
    unswapped.as_char[7] = 0x78;

    union eightbytes swapped;
    swapped.as_int = _swap64(unswapped.as_int);

    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[0], swapped.as_char[7]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[1], swapped.as_char[6]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[2], swapped.as_char[5]);
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[3], swapped.as_char[4]);    
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[4], swapped.as_char[3]);    
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[5], swapped.as_char[2]);    
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[6], swapped.as_char[1]);    
    TEST_ASSERT_EQUAL_UINT(unswapped.as_char[7], swapped.as_char[0]);    
}


