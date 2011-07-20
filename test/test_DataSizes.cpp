#include "CppUTest/TestHarness.h"
#include "ABFFIO/ProtocolStructs.h"

TEST_GROUP(DataSize)
{};

#include <limits.h>
#include <stdlib.h>

#define BIT_SIZE(type) (sizeof(type) * CHAR_BIT)

#define BIT_SIZE_EQUAL(expected_bits, type)     \
    LONGS_EQUAL(expected_bits, BIT_SIZE(type))

#define BYTE_SIZE_EQUAL(expected_bytes, type)    \
    LONGS_EQUAL(expected_bytes, sizeof(type))

TEST(DataSize, char_is_8bit)
{
    LONGS_EQUAL(8, CHAR_BIT);
}

TEST(DataSize, float_is_4bytes)
{
    BYTE_SIZE_EQUAL(4, float)
}

TEST(DataSize, double_is_8bytes)
{
    BYTE_SIZE_EQUAL(8, double)
}

TEST(DataSize, bool_is_Xbytes)
{
    BYTE_SIZE_EQUAL(1, bool);
}

TEST(DataSize, GUID_is_128bit)
{
    BIT_SIZE_EQUAL(128, GUID);
}


TEST(DataSize, ABF_Section_is_16bytes)
{
    BYTE_SIZE_EQUAL(16, ABF_Section);
}

TEST(DataSize, ABF_FileInfo_is_512bytes)
{
    BYTE_SIZE_EQUAL(512, ABF_FileInfo);
}

TEST(DataSize, ABF_ProtocolInfo_is_512bytes)
{
    BYTE_SIZE_EQUAL(512, ABF_ProtocolInfo);
}

TEST(DataSize, ABF_MathInfo_is_128bytes)
{
    BYTE_SIZE_EQUAL(128, ABF_MathInfo);
}

TEST(DataSize, ABF_ADCInfo_is_128bytes)
{
    BYTE_SIZE_EQUAL(128, ABF_ADCInfo);
}

TEST(DataSize, ABF_DACInfo_is_256bytes)
{
    BYTE_SIZE_EQUAL(256, ABF_DACInfo);
}

TEST(DataSize, ABF_EpochInfoPerDAC_is_48bytes)
{
    BYTE_SIZE_EQUAL(48, ABF_EpochInfoPerDAC);
}

TEST(DataSize, ABF_EpochInfo_is_32bytes)
{
    BYTE_SIZE_EQUAL(32, ABF_EpochInfo);
}

TEST(DataSize, ABF_StatsRegionInfo_is_128bytes)
{
    BYTE_SIZE_EQUAL(128, ABF_StatsRegionInfo);
}

TEST(DataSize, ABF_UserListInfo_is_64bytes)
{
    BYTE_SIZE_EQUAL(64, ABF_UserListInfo);
}
