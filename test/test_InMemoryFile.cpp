#include "CppUTest/TestHarness.h"

extern "C" {
#include "InMemoryFile.h"
#include <limits.h>
#include <stdio.h>
}
    
TEST_GROUP(InMemoryFile_Creation)
{
};

#define SAFE_FILE_SIZE 512
#define BAD_FILE_SIZE -5
TEST(InMemoryFile_Creation, CreateSafeSize)
{
    File memfile = InMemoryFile_Create(SAFE_FILE_SIZE);
    CHECK_TRUE(memfile != NULL);
    InMemoryFile_Destroy(memfile);
}

TEST(InMemoryFile_Creation, CreateBadFileReturnsNull)
{
    File memfile = InMemoryFile_Create(BAD_FILE_SIZE);
    POINTERS_EQUAL(NULL, memfile);
}

TEST(InMemoryFile_Creation, CreateEnormousFileReturnsNull)
{
    File memfile = InMemoryFile_Create(UINT_MAX);
    POINTERS_EQUAL(NULL, memfile);
}

#define FILE_SIZE SAFE_FILE_SIZE
TEST_GROUP(InMemoryFile)
{
    File file;
    
    
    void setup()
    {
        file = InMemoryFile_Create(FILE_SIZE);
    }

    void teardown()
    {
        InMemoryFile_Destroy(file);
    }

    void filePositionIs(filePosition expected)
    {
        LONGS_EQUAL(expected, File_getCurrentPosition(file));
    }

    void filePositionIsNot(filePosition expected)
    {
        filePosition actual = File_getCurrentPosition(file);
        if (expected == actual) {
            char fail_string[256];
            sprintf(fail_string,
                    "expected <%lu> == actual <%lu>, but should not!",
                    (long)expected, (long)actual);
            FAIL(fail_string);
        }
    }
};

TEST(InMemoryFile, BasicSeekToStart)
{
    File_seek(file, 20, 0);
    File_seek(file, 0, 0);
    filePositionIs(0);
}

TEST(InMemoryFile, BasicSeekToEnd)
{
    File_seek(file, FILE_SIZE - 1, 0);
    filePositionIs(FILE_SIZE - 1);
}

TEST(InMemoryFile, SeekPastFileEndDoesNothing)
{
    File_seek(file, 0, 0);
    File_seek(file, FILE_SIZE, 0);
    filePositionIs(0);
}

TEST(InMemoryFile, SeekPastFileStartDoesNothing)
{
    File_seek(file, 0, 0);
    File_seek(file, -5, 0);
    filePositionIs(0);
}

TEST(InMemoryFile, GetPosition)
{
    filePositionIs(0);
}

TEST(InMemoryFile, SeekToLocation)
{
    File_seek(file, 5, 0);
    filePositionIs(5);
}

TEST(InMemoryFile, SeekFromStart)
{
    File_seekFromStart(file, 10);
    filePositionIs(10);
}

TEST(InMemoryFile, SeekFromCurrent)
{
    File_seek(file, 10, 0);
    File_seekFromCurrent(file, 10);
    filePositionIs(20);
}

TEST(InMemoryFile, SeekFromEnd)
{
    File_seekFromEnd(file, 10);
    filePosition lastByte = FILE_SIZE - 1;
    filePositionIs(lastByte - 10);
}

TEST(InMemoryFile, SeekToStart)
{
    File_seek(file, 10, 0);
    filePositionIsNot(0);
    File_seekToStart(file);
    filePositionIs(0);
}

TEST(InMemoryFile, SeekToEnd)
{
    filePosition lastByte = FILE_SIZE - 1;
    File_seek(file, 0, 0);
    filePositionIsNot(lastByte);
    
    File_seekToEnd(file);
    filePositionIs(lastByte);
}

TEST(InMemoryFile, WriteSingleBlock)
{
    int8_t byteToWrite = 0x10;
    File_writeBlock(file, &byteToWrite, sizeof(byteToWrite));
    filePositionIs(1);
    BYTES_EQUAL(byteToWrite, InMemoryFile_getByteAt(file, 0));
}
