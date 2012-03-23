#include "unity.h"
#include "Stream.h"
#include "MemStream.h"
#include "streamtest_utils.h"
//#include "swap.h"

#include <string.h>

void setUp(void)
{
    MemStream_Create(STREAM_SIZE, &stream);
    err = StreamError_Unknown;
}

void tearDown(void)
{
    MemStream_Destroy(stream);
}

void test_MemStream_BasicSeekToStart(void)
{
    Stream_seek(stream, 20, 0);
    Stream_seek(stream, 0, 0);
    streamPositionIs(0);
}

void test_MemStream_BasicSeekToEnd(void)
{
    Stream_seek(stream, STREAM_SIZE - 1, 0);
    streamPositionIs(STREAM_SIZE - 1);
}

void test_MemStream_SeekPastStreamEndDoesNothing(void)
{
    Stream_seek(stream, 0, 0);
    Stream_seek(stream, STREAM_SIZE, 0);
    streamPositionIs(0);
}

void test_MemStream_SeekPastStreamStartDoesNothing(void)
{
    Stream_seek(stream, 0, 0);
    Stream_seek(stream, -5, 0);
    streamPositionIs(0);
}

void test_MemStream_GetPosition(void)
{
    streamPositionIs(0);
}

void test_MemStream_SeekToLocation(void)
{
    Stream_seek(stream, 5, 0);
    streamPositionIs(5);
}

void test_MemStream_SeekFromStart(void)
{
    Stream_seekFromStart(stream, 10);
    streamPositionIs(10);
}

void test_MemStream_SeekFromCurrent(void)
{
    Stream_seek(stream, 10, 0);
    Stream_seekFromCurrent(stream, 10);
    streamPositionIs(20);
}

void test_MemStream_SeekFromEnd(void)
{
    Stream_seekFromEnd(stream, 10);
    streamPosition lastByte = STREAM_SIZE - 1;
    streamPositionIs(lastByte - 10);
}

void test_MemStream_SeekToStart(void)
{
    Stream_seek(stream, 10, 0);
    streamPositionIsNot(0);
    Stream_seekToStart(stream);
    streamPositionIs(0);
}

void test_MemStream_SeekToEnd(void)
{
    streamPosition lastByte = STREAM_SIZE - 1;
    Stream_seek(stream, 0, 0);
    streamPositionIsNot(lastByte);

    Stream_seekToEnd(stream);
    streamPositionIs(lastByte);
}

void test_MemStream_WriteSingleChunk(void)
{
    int8_t byteToWrite = 0xFF;
    err = Stream_writeChunk(stream, &byteToWrite, sizeof(byteToWrite));
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: writeChunk did not succeed");
    streamPositionIs(1);
    TEST_ASSERT_EQUAL_HEX(byteToWrite, MemStream_getByteAt(stream, 0));
}

void test_MemStream_WriteTooBigChunkFails(void)
{
    /* write array of ones that is larger than space in stream */
    int8_t chunkToWrite[STREAM_SIZE + 1];
    memset(chunkToWrite, 1, sizeof(chunkToWrite));

    err = Stream_writeChunk(stream, chunkToWrite, sizeof(chunkToWrite));
    TEST_ASSERT_EQUAL_INT(StreamError_NoSpace, err);

    /* 1s not written to the stream */
    streamPositionIs(0);
    TEST_ASSERT_EQUAL_HEX(0, MemStream_getByteAt(stream, 0));
}

void test_MemStream_WriteMultipleChunks(void)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    err = Stream_writeMultipleChunks(stream, bytesToWrite, sizeof(int8_t), 2);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: writeMultipleChunks did not succeed");
    streamPositionIs(2);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[0], MemStream_getByteAt(stream, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 1));
}

void test_MemStream_ReadChunk(void)
{
    int8_t byteToWrite = 0xCA;
    MemStream_fillData((MemStream)stream, &byteToWrite, 1);

    int8_t byteFromStream;
    err = Stream_readChunk(stream, &byteFromStream, sizeof(int8_t));
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: readChunk did not succeed");
    TEST_ASSERT_EQUAL_HEX8(0xCA, byteFromStream);
}

void test_MemStream_ReadMultipleChunks(void)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    Stream_writeMultipleChunks(stream, bytesToWrite, sizeof(int8_t), 2);
    Stream_seekToStart(stream);

    int8_t bytesFromStream[2];
    err = Stream_readMultipleChunks(stream, bytesFromStream, sizeof(int8_t), 2);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: readMultipleChunks did not succeed");
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[0], bytesFromStream[0]);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[1], bytesFromStream[1]);
}

void test_MemStream_read_uint8(void)
{
    uint8_t byteToWrite = 0xCA;
    Stream_write_uint8(stream, byteToWrite);
    Stream_seekToStart(stream);
    
    uint8_t to;
    err = Stream_read_uint8(stream, &to);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint8 did not succeed");
    TEST_ASSERT_EQUAL_HEX8(byteToWrite, to);
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

void test_MemStream_fillData_writes_directly_to_buffer(void)
{
    uint8_t from_buf[2] = { 0xCA, 0xFE };
    MemStream_fillData((MemStream)stream, from_buf, 2);
    TEST_ASSERT_EQUAL_HEX8(0xCA, MemStream_getByteAt(stream, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 1));
}

void test_Stream_read_uint16_gets_expected_value(void)
{
    uint8_t from_buf[2] = { 0xCA, 0xFE };
    bool to_swap = false;
    MemStream_fillData((MemStream)stream, from_buf, sizeof(uint16_t));

    uint16_t to;
    err = Stream_read_uint16(stream, &to, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 did not succeed");
    
    if (ENDIAN_LITTLE == get_endian())
        TEST_ASSERT_EQUAL_HEX16(0xFECA, to);
    else
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, to);
}

void test_Stream_read_uint16_swapped(void)
{
    uint8_t from_buf[2] = { 0xFE, 0xCA };
    bool to_swap = true;
    MemStream_fillData((MemStream)stream, from_buf, sizeof(uint16_t));

    uint16_t to;
    err = Stream_read_uint16(stream, &to, to_swap);
    if (StreamError_Success != err)
        TEST_FAIL_MESSAGE("Stream error: read_uint16 did not succeed");
    
    if (ENDIAN_LITTLE == get_endian())
        TEST_ASSERT_EQUAL_HEX16(0xFECA, to);
    else
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, to);
}
