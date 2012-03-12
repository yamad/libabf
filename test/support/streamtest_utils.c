#include "streamtest_utils.h"

void streamPositionIs(streamPosition expected)
{
    streamPosition curr_pos;
    if (StreamError_Success == Stream_getCurrentPosition(stream, &curr_pos))
        TEST_ASSERT_EQUAL_INT(expected, curr_pos);
    else
        TEST_FAIL_MESSAGE("Stream error");
}

void streamPositionIsNot(streamPosition expected)
{
    streamPosition actual;
    if (StreamError_Success == Stream_getCurrentPosition(stream, &actual)) {
        if (expected == actual) {
            char fail_string[256];
            sprintf(fail_string,
                    "expected <%lu> == actual <%lu>, but should not!",
                    (long)expected, (long)actual);
            TEST_FAIL_MESSAGE(fail_string);
        }
    } else {
        TEST_FAIL_MESSAGE("Stream error");
    }
}
