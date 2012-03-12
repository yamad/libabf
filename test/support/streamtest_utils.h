#ifndef STREAMTEST_UTILS_H
#define STREAMTEST_UTILS_H

#include "unity.h"
#include "Stream.h"

#define STREAM_SIZE 512

Stream stream;
StreamError err;

void streamPositionIs(streamPosition expected);
void streamPositionIsNot(streamPosition expected);

#endif
