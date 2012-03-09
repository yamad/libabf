#include "unity.h"
#include "Stream.h"
#include "MemStream.h"

#define STREAM_SIZE 512

Stream stream;
StreamError err;

void streamPositionIs(streamPosition expected);
void streamPositionIsNot(streamPosition expected);

void streamPositionIs(streamPosition expected)
{
    streamPosition curr_pos;
    if (StreamError_Success == Stream_getCurrentPosition(stream, &curr_pos))
        TEST_ASSERT_EQUAL_INT(expected, curr_pos);
    else
        TEST_FAIL_MESSAGE("Stream error");
}

void streamPositionIsNot(streamPosition expected)
{
    streamPosition actual;
    if (StreamError_Success == Stream_getCurrentPosition(stream, &actual)) {
        if (expected == actual) {
            char fail_string[256];
            sprintf(fail_string,
                    "expected <%lu> == actual <%lu>, but should not!",
                    (long)expected, (long)actual);
            TEST_FAIL_MESSAGE(fail_string);
        }
    } else {
        TEST_FAIL_MESSAGE("Stream error");
    }
}

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
