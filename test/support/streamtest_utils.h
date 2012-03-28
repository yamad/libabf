#ifndef STREAMTEST_UTILS_H
#define STREAMTEST_UTILS_H

#include "unity.h"
#include "stream.h"
#include "swap.h"

#define STREAM_SIZE 512

stream_dt *test_stream;
STREAMERROR err;

void streamPositionIs(streampos_dt expected);
void streamPositionIsNot(streampos_dt expected);

#endif
