#include "CppUTest/TestHarness.h"

extern "C" {
#include "MemStream.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
}

TEST_GROUP(MemStream_Creation)
{
};

#define SAFE_STREAM_SIZE 512
#define BAD_STREAM_SIZE -5
TEST(MemStream_Creation, CreateSafeSize)
{
    Stream memstream = MemStream_Create(SAFE_STREAM_SIZE);
    CHECK_TRUE(memstream != NULL);
    MemStream_Destroy(memstream);
}

TEST(MemStream_Creation, CreateBadStreamReturnsNull)
{
    Stream memstream = MemStream_Create(BAD_STREAM_SIZE);
    POINTERS_EQUAL(NULL, memstream);
}

TEST(MemStream_Creation, CreateEnormousStreamReturnsNull)
{
    Stream memstream = MemStream_Create(UINT_MAX);
    POINTERS_EQUAL(NULL, memstream);
}

#define STREAM_SIZE SAFE_STREAM_SIZE
TEST_GROUP(MemStream)
{
    Stream stream;


    void setup()
    {
        stream = MemStream_Create(STREAM_SIZE);
    }

    void teardown()
    {
        MemStream_Destroy(stream);
    }

    void streamPositionIs(streamPosition expected)
    {
        LONGS_EQUAL(expected, Stream_getCurrentPosition(stream));
    }

    void streamPositionIsNot(streamPosition expected)
    {
        streamPosition actual = Stream_getCurrentPosition(stream);
        if (expected == actual) {
            char fail_string[256];
            sprintf(fail_string,
                    "expected <%lu> == actual <%lu>, but should not!",
                    (long)expected, (long)actual);
            FAIL(fail_string);
        }
    }
};

TEST(MemStream, BasicSeekToStart)
{
    Stream_seek(stream, 20, 0);
    Stream_seek(stream, 0, 0);
    streamPositionIs(0);
}

TEST(MemStream, BasicSeekToEnd)
{
    Stream_seek(stream, STREAM_SIZE - 1, 0);
    streamPositionIs(STREAM_SIZE - 1);
}

TEST(MemStream, SeekPastStreamEndDoesNothing)
{
    Stream_seek(stream, 0, 0);
    Stream_seek(stream, STREAM_SIZE, 0);
    streamPositionIs(0);
}

TEST(MemStream, SeekPastStreamStartDoesNothing)
{
    Stream_seek(stream, 0, 0);
    Stream_seek(stream, -5, 0);
    streamPositionIs(0);
}

TEST(MemStream, GetPosition)
{
    streamPositionIs(0);
}

TEST(MemStream, SeekToLocation)
{
    Stream_seek(stream, 5, 0);
    streamPositionIs(5);
}

TEST(MemStream, SeekFromStart)
{
    Stream_seekFromStart(stream, 10);
    streamPositionIs(10);
}

TEST(MemStream, SeekFromCurrent)
{
    Stream_seek(stream, 10, 0);
    Stream_seekFromCurrent(stream, 10);
    streamPositionIs(20);
}

TEST(MemStream, SeekFromEnd)
{
    Stream_seekFromEnd(stream, 10);
    streamPosition lastByte = STREAM_SIZE - 1;
    streamPositionIs(lastByte - 10);
}

TEST(MemStream, SeekToStart)
{
    Stream_seek(stream, 10, 0);
    streamPositionIsNot(0);
    Stream_seekToStart(stream);
    streamPositionIs(0);
}

TEST(MemStream, SeekToEnd)
{
    streamPosition lastByte = STREAM_SIZE - 1;
    Stream_seek(stream, 0, 0);
    streamPositionIsNot(lastByte);

    Stream_seekToEnd(stream);
    streamPositionIs(lastByte);
}

TEST(MemStream, WriteSingleChunk)
{
    int8_t byteToWrite = 0xFF;
    Stream_writeChunk(stream, &byteToWrite, sizeof(byteToWrite));
    streamPositionIs(1);
    BYTES_EQUAL(byteToWrite, MemStream_getByteAt(stream, 0));
}

TEST(MemStream, WriteTooBigChunkFails)
{
    int8_t chunkToWrite[STREAM_SIZE + 1];
    memset(chunkToWrite, 1, sizeof(chunkToWrite));
    int isDone = Stream_writeChunk(stream, chunkToWrite, sizeof(chunkToWrite));

    LONGS_EQUAL(FALSE, isDone);
    streamPositionIs(0);
    BYTES_EQUAL(0, MemStream_getByteAt(stream, 0));
}

TEST(MemStream, WriteMultipleChunks)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    Stream_writeMultipleChunks(stream, bytesToWrite, sizeof(int8_t), 2);
    streamPositionIs(2);
    BYTES_EQUAL(bytesToWrite[0], MemStream_getByteAt(stream, 0));
    BYTES_EQUAL(0xFE, MemStream_getByteAt(stream, 1));
}

TEST(MemStream, ReadChunk)
{
    int8_t byteToWrite = 0xCA;
    Stream_writeChunk(stream, &byteToWrite, sizeof(int8_t));
    Stream_seekToStart(stream);

    int8_t byteFromStream;
    Stream_readChunk(stream, &byteFromStream, sizeof(int8_t));
    BYTES_EQUAL(0xCA, byteFromStream);
}

TEST(MemStream, ReadMultipleChunks)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    Stream_writeMultipleChunks(stream, bytesToWrite, sizeof(int8_t), 2);

    Stream_seekToStart(stream);
    int8_t bytesFromStream[2];
    Stream_readMultipleChunks(stream, bytesFromStream, sizeof(int8_t), 2);
    BYTES_EQUAL(bytesToWrite[0], bytesFromStream[0]);
    BYTES_EQUAL(bytesToWrite[1], bytesFromStream[1]);
}
