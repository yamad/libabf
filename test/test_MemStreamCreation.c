#include "unity.h"
#include "MemStream.h"

#include <limits.h>

#define SAFE_STREAM_SIZE 512
#define BAD_STREAM_SIZE -5

void setUp(void) 
{
}

void tearDown(void)
{
}

void test_MemStream_Creation_CreateSafeSize(void)
{
    Stream memstream;
    StreamError err;
    err = MemStream_Create(SAFE_STREAM_SIZE, &memstream);
    TEST_ASSERT_EQUAL_INT(err, StreamError_Success);
}

void test_MemStream_Creation_CreateBadStreamReturnsMemoryError(void)
{
    Stream memstream;
    StreamError err = MemStream_Create(BAD_STREAM_SIZE, &memstream);
    TEST_ASSERT_EQUAL_INT(err, StreamError_NoMemory);
    TEST_ASSERT_NULL(memstream);
}

void test_MemStream_Creation_CreateEnormousStreamReturnsMemoryError(void)
{
    Stream memstream;
    StreamError err = MemStream_Create(UINT_MAX, &memstream);
    TEST_ASSERT_EQUAL_INT(err, StreamError_NoMemory);
    TEST_ASSERT_NULL(memstream);
}
