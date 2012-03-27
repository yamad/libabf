#include "unity.h"
#include "MemStream.h"

#include <limits.h>

#define SAFE_STREAM_SIZE 512
#define BAD_STREAM_SIZE -5

stream_dt *memstream;
STREAMERROR err;

void setUp(void) 
{
}

void tearDown(void)
{
}

void test_MemStream_Creation_CreateSafeSize(void)
{
    err = memstream_create(SAFE_STREAM_SIZE, &memstream);
    TEST_ASSERT_EQUAL_INT(err, STREAMERROR_SUCCESS);
}

void test_MemStream_Creation_CreateBadStreamReturnsMemoryError(void)
{
    err = memstream_create(BAD_STREAM_SIZE, &memstream);
    TEST_ASSERT_EQUAL_INT(err, STREAMERROR_NOMEMORY);
    TEST_ASSERT_NULL(memstream);
}

void test_MemStream_Creation_CreateEnormousStreamReturnsMemoryError(void)
{
    err = memstream_create(UINT_MAX, &memstream);
    TEST_ASSERT_EQUAL_INT(err, STREAMERROR_NOMEMORY);
    TEST_ASSERT_NULL(memstream);
}
