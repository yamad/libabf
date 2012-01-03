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
    Stream memstream = MemStream_Create(SAFE_STREAM_SIZE);
    TEST_ASSERT_NOT_NULL(memstream);
    MemStream_Destroy(memstream);
}

void test_MemStream_Creation_CreateBadStreamReturnsNull(void)
{
    Stream memstream = MemStream_Create(BAD_STREAM_SIZE);
    TEST_ASSERT_NULL(memstream);
}

void test_MemStream_Creation_CreateEnormousStreamReturnsNull(void)
{
    Stream memstream = MemStream_Create(UINT_MAX);
    TEST_ASSERT_NULL(memstream);
}
