#include <stdlib.h>
#include "unity.h"

#include "abf2.h"
#include "abf2_stream.h"
#include "stream.h"
#include "memstream.h"
#include "deserialize.h"
#include "swap.h"

#include "streamtest_utils.h"

void setUp(void)
{
    memstream_create(STREAM_SIZE, &test_stream);
    err = StreamError_Unknown;
}

void tearDown(void)
{
    memstream_destroy(test_stream);
}

void test_abf2_stream_read_guid(void)
{
    /* example GUID from MSDN docs: 6B29FC40-CA47-1067-B31D-00DD010662DA */
    char bytes[16] = {0x6B, 0x29, 0xFC, 0x40,
                      0xCA, 0x47,
                      0x10, 0x67,
                      0xB3, 0x1D,
                      0x00, 0xDD, 0x01, 0x06, 0x62, 0xDA};
    char *buf = bytes;
    memstream_fillData((memstream_dt*)test_stream, buf, 16);
    bool to_swap = false;

    struct guid guid;
    err = abf2_stream_read_guid(test_stream, &guid, to_swap);
    if (StreamError_Success != err) {
        TEST_FAIL_MESSAGE("abf2_stream_read_guid failed");
    }
    streampos_dt currpos;
    stream_tell(test_stream, &currpos);
    TEST_ASSERT_EQUAL_INT(16, currpos);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0x40FC296B, guid.Data1);
        TEST_ASSERT_EQUAL_HEX16(0x47CA, guid.Data2);
        TEST_ASSERT_EQUAL_HEX16(0x6710, guid.Data3);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0x6B29FC40, guid.Data1);
        TEST_ASSERT_EQUAL_HEX16(0xCA47, guid.Data2);
        TEST_ASSERT_EQUAL_HEX16(0x1067, guid.Data3);
    }
    TEST_ASSERT_EQUAL_HEX8(0xB3, guid.Data4[0]);
    TEST_ASSERT_EQUAL_HEX8(0x1D, guid.Data4[1]);
    TEST_ASSERT_EQUAL_HEX8(0x00, guid.Data4[2]);
    TEST_ASSERT_EQUAL_HEX8(0xDD, guid.Data4[3]);
    TEST_ASSERT_EQUAL_HEX8(0x01, guid.Data4[4]);
    TEST_ASSERT_EQUAL_HEX8(0x06, guid.Data4[5]);
    TEST_ASSERT_EQUAL_HEX8(0x62, guid.Data4[6]);
    TEST_ASSERT_EQUAL_HEX8(0xDA, guid.Data4[7]);
}

void test_abf2_stream_read_guid_swap(void)
{
    /* example GUID from MSDN docs: 6B29FC40-CA47-1067-B31D-00DD010662DA */
    char bytes[16] = {0x6B, 0x29, 0xFC, 0x40,
                      0xCA, 0x47,
                      0x10, 0x67,
                      0xB3, 0x1D,
                      0x00, 0xDD, 0x01, 0x06, 0x62, 0xDA};
    char *buf = bytes;
    memstream_fillData((memstream_dt*)test_stream, buf, 16);
    bool to_swap = true;

    struct guid guid;
    err = abf2_stream_read_guid(test_stream, &guid, to_swap);
    if (StreamError_Success != err) {
        TEST_FAIL_MESSAGE("abf2_stream_read_guid failed");
    }
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0x6B29FC40, guid.Data1);
        TEST_ASSERT_EQUAL_HEX16(0xCA47, guid.Data2);
        TEST_ASSERT_EQUAL_HEX16(0x1067, guid.Data3);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0x40FC296B, guid.Data1);
        TEST_ASSERT_EQUAL_HEX16(0x47CA, guid.Data2);
        TEST_ASSERT_EQUAL_HEX16(0x6710, guid.Data3);
    }
    TEST_ASSERT_EQUAL_HEX8(0xB3, guid.Data4[0]);
    TEST_ASSERT_EQUAL_HEX8(0x1D, guid.Data4[1]);
    TEST_ASSERT_EQUAL_HEX8(0x00, guid.Data4[2]);
    TEST_ASSERT_EQUAL_HEX8(0xDD, guid.Data4[3]);
    TEST_ASSERT_EQUAL_HEX8(0x01, guid.Data4[4]);
    TEST_ASSERT_EQUAL_HEX8(0x06, guid.Data4[5]);
    TEST_ASSERT_EQUAL_HEX8(0x62, guid.Data4[6]);
    TEST_ASSERT_EQUAL_HEX8(0xDA, guid.Data4[7]);
}
