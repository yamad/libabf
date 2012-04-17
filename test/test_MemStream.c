#include "streamtest_utils.h"
#include "stream.h"
#include "memstream.h"
#include "swap.h"

#include <string.h>

void setUp(void)
{
    memstream_create(STREAM_SIZE, &test_stream);
    err = StreamError_Unknown;
}

void tearDown(void)
{
    memstream_destroy(test_stream);
}

void test_memstream_BasicSeekToStart(void)
{
    stream_seek(test_stream, 20, 0);
    stream_seek(test_stream, 0, 0);
    streamPositionIs(0);
}

void test_memstream_BasicSeekToEnd(void)
{
    stream_seek(test_stream, STREAM_SIZE - 1, 0);
    streamPositionIs(STREAM_SIZE - 1);
}

void test_memstream_SeekPastStreamEndDoesNothing(void)
{
    stream_seek(test_stream, 0, 0);
    stream_seek(test_stream, STREAM_SIZE, 0);
    streamPositionIs(0);
}

void test_memstream_SeekPastStreamStartDoesNothing(void)
{
    stream_seek(test_stream, 0, 0);
    stream_seek(test_stream, -5, 0);
    streamPositionIs(0);
}

void test_memstream_GetPosition(void)
{
    streamPositionIs(0);
}

void test_memstream_SeekToLocation(void)
{
    stream_seek(test_stream, 5, 0);
    streamPositionIs(5);
}

void test_memstream_SeekFromStart(void)
{
    stream_seekFromStart(test_stream, 10);
    streamPositionIs(10);
}

void test_memstream_SeekFromCurrent(void)
{
    stream_seek(test_stream, 10, 0);
    stream_seekFromCurrent(test_stream, 10);
    streamPositionIs(20);
}

void test_memstream_SeekFromEnd(void)
{
    stream_seekFromEnd(test_stream, 10);
    streampos_dt lastByte = STREAM_SIZE - 1;
    streamPositionIs(lastByte - 10);
}

void test_memstream_SeekToStart(void)
{
    stream_seek(test_stream, 10, 0);
    streamPositionIsNot(0);
    stream_seekToStart(test_stream);
    streamPositionIs(0);
}

void test_memstream_SeekToEnd(void)
{
    streampos_dt lastByte = STREAM_SIZE - 1;
    stream_seek(test_stream, 0, 0);
    streamPositionIsNot(lastByte);

    stream_seekToEnd(test_stream);
    streamPositionIs(lastByte);
}

void test_memstream_write_writes_byte(void)
{
    int8_t byteToWrite = 0xFF;
    err = stream_write(test_stream, &byteToWrite, sizeof(byteToWrite));
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: write did not succeed");
    streamPositionIs(1);
    TEST_ASSERT_EQUAL_HEX(byteToWrite, memstream_getByteAt(test_stream, 0));
}

void test_memstream_write_toolarge_failure(void)
{
    /* write array of ones that is larger than space in stream */
    int8_t chunkToWrite[STREAM_SIZE + 1];
    memset(chunkToWrite, 1, sizeof(chunkToWrite));

    err = stream_write(test_stream, chunkToWrite, sizeof(chunkToWrite));
    TEST_ASSERT_EQUAL_INT(StreamError_NoSpace, err);

    /* 1s not written to the stream */
    streamPositionIs(0);
    TEST_ASSERT_EQUAL_HEX(0, memstream_getByteAt(test_stream, 0));
}

void test_memstream_writen(void)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    err = stream_writen(test_stream, bytesToWrite, sizeof(int8_t), 2);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: writendid not succeed");
    streamPositionIs(2);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[0], memstream_getByteAt(test_stream, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
}

void test_memstream_read(void)
{
    int8_t byteToWrite = 0xCA;
    memstream_fillData((memstream_dt*)test_stream, &byteToWrite, 1);

    int8_t byteFromStream;
    err = stream_read(test_stream, &byteFromStream, sizeof(int8_t));
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read did not succeed");
    TEST_ASSERT_EQUAL_HEX8(0xCA, byteFromStream);
}

void test_memstream_readn(void)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    stream_writen(test_stream, bytesToWrite, sizeof(int8_t), 2);
    stream_seekToStart(test_stream);

    int8_t bytesFromStream[2];
    err = stream_readn(test_stream, bytesFromStream, sizeof(int8_t), 2);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: readn did not succeed");
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[0], bytesFromStream[0]);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[1], bytesFromStream[1]);
}

void test_memstream_read_uint8(void)
{
    uint8_t byteToWrite = 0x02;
    memstream_fillData((memstream_dt*)test_stream, &byteToWrite, 1);
    stream_seekToStart(test_stream);

    uint8_t to;
    err = stream_read_uint8(test_stream, &to);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint8 did not succeed");
    TEST_ASSERT_EQUAL_HEX8(byteToWrite, to);
}

void test_memstream_fillData_writes_directly_to_buffer(void)
{
    uint8_t from_buf[2] = { 0xCA, 0xFE };
    memstream_fillData((memstream_dt*)test_stream, from_buf, 2);
    TEST_ASSERT_EQUAL_HEX8(0xCA, memstream_getByteAt(test_stream, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFE, memstream_getByteAt(test_stream, 1));
}

void test_stream_read_uint16_gets_expected_value(void)
{
    uint8_t from_buf[2] = { 0xCA, 0xFE };
    bool to_swap = false;
    memstream_fillData((memstream_dt*)test_stream, from_buf, sizeof(uint16_t));

    uint16_t to;
    err = stream_read_uint16(test_stream, &to, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 did not succeed");

    if (ENDIAN_LITTLE == get_endian())
        TEST_ASSERT_EQUAL_HEX16(0xFECA, to);
    else
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, to);
}

void test_stream_read_uint16_swapped(void)
{
    uint8_t from_buf[2] = { 0xFE, 0xCA };
    bool to_swap = true;
    memstream_fillData((memstream_dt*)test_stream, from_buf, sizeof(uint16_t));

    uint16_t to;
    err = stream_read_uint16(test_stream, &to, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 did not succeed");

    if (ENDIAN_LITTLE == get_endian())
        TEST_ASSERT_EQUAL_HEX16(0xFECA, to);
    else
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, to);
}
