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
