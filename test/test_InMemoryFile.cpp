#include "CppUTest/TestHarness.h"

extern "C" {
#include "InMemoryFile.h"
#include <limits.h>
}
    
TEST_GROUP(InMemoryFile)
{
};

#define SAFE_FILE_SIZE 512
#define BAD_FILE_SIZE -5
TEST(InMemoryFile, CreateSafeSize)
{
    File memfile = InMemoryFile_Create(SAFE_FILE_SIZE);
    CHECK_TRUE(memfile != NULL);
    InMemoryFile_Destroy(memfile);
}

TEST(InMemoryFile, CreateBadFileReturnsNull)
{
    File memfile = InMemoryFile_Create(BAD_FILE_SIZE);
    POINTERS_EQUAL(NULL, memfile);
}

TEST(InMemoryFile, CreateEnormousFileReturnsNull)
{
    File memfile = InMemoryFile_Create(UINT_MAX);
    POINTERS_EQUAL(NULL, memfile);
}

TEST(InMemoryFile, GetPosition)
{
    File memfile = InMemoryFile_Create(512);
    filePosition position = File_getCurrentPosition(memfile);
    LONGS_EQUAL(0, position);
    InMemoryFile_Destroy(memfile);
}

TEST(InMemoryFile, SeekToLocation)
{
    File memfile = InMemoryFile_Create(512);
    File_seek(memfile, 5, 0);
    filePosition position = File_getCurrentPosition(memfile);
    LONGS_EQUAL(5, position);
    InMemoryFile_Destroy(memfile);
}

TEST(InMemoryFile, WriteSingleBlock)
{
    
}
