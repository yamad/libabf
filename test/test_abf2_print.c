#include <stdlib.h>
#include "unity.h"

#include "abf2.h"
#include "abf2_print.h"
#include "deserialize.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_repeated_string(void)
{
    char *dest = get_repeated_string('A', 4);
    TEST_ASSERT_EQUAL_STRING("AAAA", dest);
    free(dest);
}
