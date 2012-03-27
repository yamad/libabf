#ifndef STREAMTEST_UTILS_H
#define STREAMTEST_UTILS_H

#include "unity.h"
#include "Stream.h"
#include "swap.h"

#define STREAM_SIZE 512

stream_dt *test_stream;
STREAMERROR err;

void streamPositionIs(streamPosition expected);
void streamPositionIsNot(streamPosition expected);

#endif
