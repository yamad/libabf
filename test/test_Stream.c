#include "unity.h"
#include "Stream.h"
#include "MemStream.h"

#include "streamtest_utils.h"
#include "swap.h"

void setUp(void)
{
    MemStream_Create(STREAM_SIZE, &stream);
    err = StreamError_Unknown;
}

void tearDown(void)
{
    MemStream_Destroy(stream);
}

void test_Stream_write_uint8_writes_a_byte(void)
{
    uint8_t byteToWrite = 0xCA;
    err = Stream_write_uint8(stream, byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    TEST_ASSERT_EQUAL_HEX8(byteToWrite, MemStream_getByteAt(stream, 0));
}

void test_Stream_write_uint8_advances_pos(void)
{
    uint8_t byteToWrite = 0xCA;
    err = Stream_write_uint8(stream, byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    streamPositionIs(1);
}

void test_Stream_write_int8_writes_a_byte(void)
{
    int8_t byteToWrite = 0xCA;
    err = Stream_write_int8(stream, byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_int8 did not succeed");
    TEST_ASSERT_EQUAL_INT8(byteToWrite, MemStream_getByteAt(stream, 0));
}

void test_Stream_write_uint16_writes_expected_order(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = Stream_write_uint16(stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");

    Stream_seekToStart(stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xCA, MemStream_getByteAt(stream, 1));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xCA, MemStream_getByteAt(stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 1));
    }
}

void test_Stream_write_uint16_can_swap(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = true;
    err = Stream_write_uint16(stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");

    Stream_seekToStart(stream);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX8(0xCA, MemStream_getByteAt(stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 1));
    } else {
        TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 0));
        TEST_ASSERT_EQUAL_HEX8(0xCA, MemStream_getByteAt(stream, 1));
    }
}

void test_Stream_write_uint16_advances_pos(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = Stream_write_uint16(stream, bytesToWrite, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");
    streamPositionIs(2);
}

void test_Stream_read_uint16_gets_expected_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    MemStream_fillData((MemStream)stream, bytesToWrite, 2);
    bool to_swap = false;

    uint16_t result;
    err = Stream_read_uint16(stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);        
    }
}

void test_Stream_read_uint16_gets_swapped_value(void)
{
    uint8_t bytesToWrite[2] = { 0xCA, 0xFE };
    MemStream_fillData((MemStream)stream, bytesToWrite, 2);
    bool to_swap = true;

    uint16_t result;
    err = Stream_read_uint16(stream, &result, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 failed");
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);        
    }
}

/* void test_Stream_write_uint8_advances_pos(void) */
/* { */
/*     uint8_t byteToWrite = 0xCA; */
/*     err = Stream_write_uint8(stream, &byteToWrite); */
/*     if (StreamError_Success != err) */
/*         TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed"); */
/*     streamPositionIs(1); */
/* } */

/* void test_Stream_write_int8_writes_a_byte(void) */
/* { */
/*     int8_t byteToWrite = 0xCA; */
/*     err = Stream_write_int8(stream, &byteToWrite); */
/*     if (StreamError_Success != err) */
/*         TEST_FAIL_MESSAGE("Stream error: write_int8 did not succeed"); */
/*     TEST_ASSERT_EQUAL_INT8(byteToWrite, MemStream_getByteAt(stream, 0)); */
/* } */
