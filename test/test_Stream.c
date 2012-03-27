#include "unity.h"
#include "stream.h"
#include "memstream.h"

#include "streamtest_utils.h"
#include "swap.h"

void setUp(void)
{
    memstream_create(STREAM_SIZE, &test_stream);
    err = STREAMERROR_UNKNOWN;
}

void tearDown(void)
{
    memstream_destroy(test_stream);
}

void test_stream_write_uint8_writes_a_byte(void)
{
    uint8_t byteToWrite = 0xCA;
    err = stream_write_uint8(test_stream, byteToWrite);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    TEST_ASSERT_EQUAL_HEX8(byteToWrite, memstream_getByteAt(test_stream, 0));
}

void test_stream_write_uint8_advances_pos(void)
{
    uint8_t byteToWrite = 0xCA;
    err = stream_write_uint8(test_stream, byteToWrite);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    streamPositionIs(1);
}

void test_stream_write_int8_writes_a_byte(void)
{
    int8_t byteToWrite = 0xCA;
    err = stream_write_int8(test_stream, byteToWrite);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int8 did not succeed");
    TEST_ASSERT_EQUAL_INT8(byteToWrite, memstream_getByteAt(test_stream, 0));
}

void test_stream_write_uint16_writes_expected_order(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = stream_write_uint16(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 1));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
    }
}

void test_stream_write_uint16_can_swap(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = true;
    err = stream_write_uint16(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 1));
    }
}

void test_stream_write_uint16_advances_pos(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = stream_write_uint16(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");
    streamPositionIs(2);
}

void test_stream_write_int16_writes_expected_order(void)
{
    int16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = stream_write_int16(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int16 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 1));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
    }
}

void test_stream_write_int16_can_swap(void)
{
    int16_t bytesToWrite = 0xCAFE;
    bool to_swap = true;
    err = stream_write_int16(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int16 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 1));
    }
}

void test_stream_write_int16_advances_pos(void)
{
    int16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = stream_write_int16(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int16 did not succeed");
    streamPositionIs(2);
}

void test_stream_write_uint32_writes_expected_order(void)
{
    uint32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = false;
    err = stream_write_uint32(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint32 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 3));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 3));
    }
}

void test_stream_write_uint32_can_swap(void)
{
    uint32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = true;
    err = stream_write_uint32(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint32 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 3));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 3));
    }
}

void test_stream_write_uint32_advances_pos(void)
{
    uint32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = false;
    err = stream_write_uint32(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint32 did not succeed");
    streamPositionIs(4);
}

void test_stream_write_int32_writes_expected_order(void)
{
    int32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = false;
    err = stream_write_int32(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int32 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 3));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 3));
    }
}

void test_stream_write_int32_can_swap(void)
{
    int32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = true;
    err = stream_write_int32(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int32 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 3));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xEF, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xBE, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 3));
    }
}

void test_stream_write_int32_advances_pos(void)
{
    int32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = false;
    err = stream_write_int32(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int32 did not succeed");
    streamPositionIs(4);
}

void test_stream_write_uint64_writes_expected_order(void)
{
    uint64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = false;
    err = stream_write_uint64(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint64 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 7));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 7));
    }
}

void test_stream_write_uint64_can_swap(void)
{
    uint64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = true;
    err = stream_write_uint64(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint64 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 7));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 7));
    }
}

void test_stream_write_uint64_advances_pos(void)
{
    uint64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = false;
    err = stream_write_uint64(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint64 did not succeed");
    streamPositionIs(8);
}

void test_stream_write_int64_writes_expected_order(void)
{
    int64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = false;
    err = stream_write_int64(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int64 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 7));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 7));
    }
}

void test_stream_write_int64_can_swap(void)
{
    int64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = true;
    err = stream_write_int64(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int64 did not succeed");

    stream_seekToStart(test_stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 7));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0x08, memstream_getByteAt(test_stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0x07, memstream_getByteAt(test_stream, 1));
        TEST_ASSERT_EQUAL_HEX8(0x06, memstream_getByteAt(test_stream, 2));
        TEST_ASSERT_EQUAL_HEX8(0x05, memstream_getByteAt(test_stream, 3));
        TEST_ASSERT_EQUAL_HEX8(0x04, memstream_getByteAt(test_stream, 4));
        TEST_ASSERT_EQUAL_HEX8(0x03, memstream_getByteAt(test_stream, 5));
        TEST_ASSERT_EQUAL_HEX8(0x02, memstream_getByteAt(test_stream, 6));
        TEST_ASSERT_EQUAL_HEX8(0x01, memstream_getByteAt(test_stream, 7));
    }
}

void test_stream_write_int64_advances_pos(void)
{
    int64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = false;
    err = stream_write_int64(test_stream, bytesToWrite, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: write_int64 did not succeed");
    streamPositionIs(8);
}

void test_stream_read_uint16_gets_expected_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 2);
    bool to_swap = false;

    uint16_t result;
    err = stream_read_uint16(test_stream, &result, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);        
    }
}

void test_stream_read_uint16_gets_swapped_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 2);
    bool to_swap = true;

    uint16_t result;
    err = stream_read_uint16(test_stream, &result, to_swap);
    if (STREAMERROR_SUCCESS != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);        
    }
}
