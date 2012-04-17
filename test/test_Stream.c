#include "unity.h"
#include "stream.h"
#include "memstream.h"

#include "streamtest_utils.h"
#include "swap.h"

void setUp(void)
{
    memstream_create(STREAM_SIZE, &test_stream);
    err = StreamError_Unknown;
}

void tearDown(void)
{
    memstream_destroy(test_stream);
}

void test_stream_write_uint8_writes_a_byte(void)
{
    uint8_t byteToWrite = 0xCA;
    err = stream_write_uint8(test_stream, byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    TEST_ASSERT_EQUAL_HEX8(byteToWrite, memstream_getByteAt(test_stream, 0));
}

void test_stream_write_uint8_advances_pos(void)
{
    uint8_t byteToWrite = 0xCA;
    err = stream_write_uint8(test_stream, byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    streamPositionIs(1);
}

void test_stream_write_int8_writes_a_byte(void)
{
    int8_t byteToWrite = 0xCA;
    err = stream_write_int8(test_stream, byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_int8 did not succeed");
    TEST_ASSERT_EQUAL_INT8(byteToWrite, memstream_getByteAt(test_stream, 0));
}

void test_stream_write_uint16_writes_expected_order(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = stream_write_uint16(test_stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");
    streamPositionIs(2);
}

void test_stream_write_int16_writes_expected_order(void)
{
    int16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = stream_write_int16(test_stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_int16 did not succeed");
    streamPositionIs(2);
}

void test_stream_write_uint32_writes_expected_order(void)
{
    uint32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = false;
    err = stream_write_uint32(test_stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint32 did not succeed");
    streamPositionIs(4);
}

void test_stream_write_int32_writes_expected_order(void)
{
    int32_t bytesToWrite = 0xCAFEBEEF;
    bool to_swap = false;
    err = stream_write_int32(test_stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_int32 did not succeed");
    streamPositionIs(4);
}

void test_stream_write_uint64_writes_expected_order(void)
{
    uint64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = false;
    err = stream_write_uint64(test_stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint64 did not succeed");
    streamPositionIs(8);
}

void test_stream_write_int64_writes_expected_order(void)
{
    int64_t bytesToWrite = 0x0102030405060708;
    bool to_swap = false;
    err = stream_write_int64(test_stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_int64 did not succeed");
    streamPositionIs(8);
}

void test_stream_read_uint8_gets_expected_value(void)
{
    uint8_t byteToWrite = 0xCA;
    memstream_fillData((memstream_dt*)test_stream, &byteToWrite, 1);

    uint8_t result;
    err = stream_read_uint8(test_stream, &result);
    TEST_ASSERT_EQUAL_HEX8(0xCA, result);
}

void test_stream_read_int8_gets_expected_value(void)
{
    uint8_t byteToWrite = 0xCA;
    memstream_fillData((memstream_dt*)test_stream, &byteToWrite, 1);

    int8_t result;
    err = stream_read_int8(test_stream, &result);
    TEST_ASSERT_EQUAL_HEX8(0xCA, result);
}

void test_stream_read_uint16_gets_expected_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 2);
    bool to_swap = false;

    uint16_t result;
    err = stream_read_uint16(test_stream, &result, to_swap);
    if (StreamError_Success != err)
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
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    }
}

void test_stream_read_int16_gets_expected_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 2);
    bool to_swap = false;

    int16_t result;
    err = stream_read_int16(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_int16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    }
}

void test_stream_read_int16_gets_swapped_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 2);
    bool to_swap = true;

    int16_t result;
    err = stream_read_int16(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_int16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    }
}

void test_stream_read_uint32_gets_expected_value(void)
{
    uint8_t bytesToWrite[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 4);
    bool to_swap = false;

    uint32_t result;
    err = stream_read_uint32(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint32 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    }
}

void test_stream_read_uint32_gets_swapped_value(void)
{
    uint8_t bytesToWrite[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 4);
    bool to_swap = true;

    uint32_t result;
    err = stream_read_uint32(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint32 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    }
}

void test_stream_read_int32_gets_expected_value(void)
{
    uint8_t bytesToWrite[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 4);
    bool to_swap = false;

    int32_t result;
    err = stream_read_int32(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_int32 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    }
}

void test_stream_read_int32_gets_swapped_value(void)
{
    uint8_t bytesToWrite[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 4);
    bool to_swap = true;

    int32_t result;
    err = stream_read_int32(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_int32 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    }
}

void test_stream_read_uint64_gets_expected_value(void)
{
    uint8_t bytesToWrite[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 8);
    bool to_swap = false;

    uint64_t result;
    err = stream_read_uint64(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint64 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0x0706050403020100, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0x0001020304050607, result);
    }
}

void test_stream_read_uint64_gets_swapped_value(void)
{
    uint8_t bytesToWrite[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 8);
    bool to_swap = true;

    uint64_t result;
    err = stream_read_uint64(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint64 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0x0001020304050607, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0x0706050403020100, result);
    }
}

void test_stream_read_int64_gets_expected_value(void)
{
    uint8_t bytesToWrite[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 8);
    bool to_swap = false;

    int64_t result;
    err = stream_read_int64(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_int64 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0x0706050403020100, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0x0001020304050607, result);
    }
}

void test_stream_read_int64_gets_swapped_value(void)
{
    uint8_t bytesToWrite[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    memstream_fillData((memstream_dt*)test_stream, bytesToWrite, 8);
    bool to_swap = true;

    int64_t result;
    err = stream_read_int64(test_stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_int64 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0x0001020304050607, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0x0706050403020100, result);
    }
}
