#include "unity.h"
#include "ABF2_struct.h"

#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#define PACK_STRUCT

#define BIT_SIZE(type) (sizeof(type) * CHAR_BIT)

#define BIT_SIZE_EQUAL(expected_bits, type)                     \
    TEST_ASSERT_EQUAL_INT(expected_bits, BIT_SIZE(type))

#define BYTE_SIZE_EQUAL(expected_bytes, type)           \
    TEST_ASSERT_EQUAL_INT(expected_bytes, sizeof(type))

void setUp(void) {}
void tearDown(void) {}

void test_char_is_8bits(void)
{
    TEST_ASSERT_EQUAL_INT(8, CHAR_BIT);
}

void test_float_is_4bytes(void)
{
    BYTE_SIZE_EQUAL(4, float);
}

void test_double_is_8bytes(void)
{
    BYTE_SIZE_EQUAL(8, double);
}

void test_uint64_is_8bytes(void)
{
    BYTE_SIZE_EQUAL(8, uint64_t);
}

void test_bool_is_1byte(void)
{
    BYTE_SIZE_EQUAL(1, bool);
}

void test_GUID_is_128bit(void)
{
    BIT_SIZE_EQUAL(128, GUID);
}

void test_ABF_Section_is_16bytes(void)
{
    BYTE_SIZE_EQUAL(16, ABF_Section);
}

void test_ABF_FileInfo_is_512bytes(void)
{
    TEST_IGNORE();
    BYTE_SIZE_EQUAL(512, ABF_FileInfo);
}

void test_ABF_ProtocolInfo_is_512bytes(void)
{
    TEST_IGNORE();
    BYTE_SIZE_EQUAL(512, ABF_ProtocolInfo);
}

void test_ABF_MathInfo_is_128bytes(void)
{
    BYTE_SIZE_EQUAL(128, ABF_MathInfo);
}

void test_ABF_ADCInfo_is_128bytes(void)
{
    TEST_IGNORE();
    BYTE_SIZE_EQUAL(128, ABF_ADCInfo);
}

void test_ABF_DACInfo_is_256bytes(void)
{
    TEST_IGNORE();
    BYTE_SIZE_EQUAL(256, ABF_DACInfo);
}

void test_ABF_EpochInfoPerDAC_is_48bytes(void)
{
    TEST_IGNORE();
    BYTE_SIZE_EQUAL(48, ABF_EpochInfoPerDAC);
}

void test_ABF_EpochInfo_is_32bytes(void)
{
    BYTE_SIZE_EQUAL(32, ABF_EpochInfo);
}

void test_ABF_StatsRegionInfo_is_128bytes(void)
{
    BYTE_SIZE_EQUAL(128, ABF_StatsRegionInfo);
}

void test_ABF_UserListInfo_is_64bytes(void)
{
    BYTE_SIZE_EQUAL(64, ABF_UserListInfo);
}
