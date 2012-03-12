#include "unity.h"
#include "Stream.h"
#include "MemStream.h"

#include "streamtest_utils.h"

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
    err = Stream_write_uint8(stream, &byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    TEST_ASSERT_EQUAL_HEX8(byteToWrite, MemStream_getByteAt(stream, 0));
}

void test_Stream_write_uint8_advances_pos(void)
{
    uint8_t byteToWrite = 0xCA;
    err = Stream_write_uint8(stream, &byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint8 did not succeed");
    streamPositionIs(1);
}

void test_Stream_write_int8_writes_a_byte(void)
{
    int8_t byteToWrite = 0xCA;
    err = Stream_write_int8(stream, &byteToWrite);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_int8 did not succeed");
    TEST_ASSERT_EQUAL_INT8(byteToWrite, MemStream_getByteAt(stream, 0));
}

void test_Stream_write_uint16_advances_pos(void)
{
    uint16_t bytesToWrite = 0xCAFE;
    bool to_swap = false;
    err = Stream_write_uint16(stream, &bytesToWrite, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write_uint16 did not succeed");
    streamPositionIs(2);
//    TEST_ASSERT_EQUAL_HEX16(0xCAFE, MemStream_getByteAt(stream, 0));
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
