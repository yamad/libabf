#include <stdlib.h>

#include "unity.h"
#include "abf_string.h"

struct abf_strcache sc;

void setUp(void) {
    abf_strcache_init(&sc);
}
void tearDown(void) {
    abf_strcache_destroy(&sc);
}

void test_abf_read_strings_gets_all_strings(void) {
    uint8_t bytes[] = { 0x63,0x6C,0x61,0x6D,0x70,0x65,0x78,0x00,0x43,0x3A,
                        0x5C,0x44,0x6F,0x63,0x75,0x6D,0x65,0x6E,0x74,0x73,
                        0x20,0x61,0x6E,0x64,0x20,0x53,0x65,0x74,0x74,0x69,
                        0x6E,0x67,0x73,0x5C,0x4A,0x61,0x73,0x6F,0x6E,0x20,
                        0x59,0x61,0x6D,0x61,0x64,0x61,0x2D,0x48,0x61,0x6E,
                        0x66,0x66,0x5C,0x4D,0x79,0x20,0x44,0x6F,0x63,0x75,
                        0x6D,0x65,0x6E,0x74,0x73,0x5C,0x70,0x72,0x6F,0x74,
                        0x6F,0x63,0x6F,0x6C,0x73,0x5C,0x63,0x63,0x5F,0x62,
                        0x6C,0x6F,0x77,0x6F,0x66,0x66,0x2E,0x70,0x72,0x6F,
                        0x00,0x56,0x6D,0x5F,0x43,0x43,0x5F,0x37,0x30,0x30,
                        0x42,0x00,0x6D,0x56,0x00,0x49,0x6D,0x5F,0x43,0x43,
                        0x5F,0x37,0x30,0x30,0x42,0x00,0x70,0x41,0x00,0x43,
                        0x43,0x5F,0x43,0x6D,0x64,0x30,0x00,0x6E,0x41,0x00,
                        0x43,0x43,0x5F,0x43,0x6D,0x64,0x31,0x00,0x70,0x41,
                        0x00,0x4F,0x55,0x54,0x20,0x32,0x00,0x56,0x00,0x4F,
                        0x55,0x54,0x20,0x33,0x00,0x56,0x00 };
//    abf_read_strings(&sc, bytes, 15);
//    TEST_ASSERT_EQUAL_STRING("clampex", *(sc->strings));
}

void test_abf_strcache_init(void) {
    TEST_ASSERT_EQUAL_INT(0, sc.count);
}

void test_abf_strcache_add(void) {
    char *expected = "Hello, world!";
    abf_strcache_add(&sc, expected);
    TEST_ASSERT_EQUAL_INT(1, sc.count);
    TEST_ASSERT_EQUAL_STRING(expected, *(sc.strings));
}

void test_abf_strcache_add_multiple(void) {
    char *str1 = "Hello, world!";
    char *str2 = "Goodbye, world!";

    abf_strcache_add(&sc, str1);
    abf_strcache_add(&sc, str2);

    TEST_ASSERT_EQUAL_INT(2, sc.count);
    TEST_ASSERT_EQUAL_STRING(str1, sc.strings[0]);
    TEST_ASSERT_EQUAL_STRING(str2, sc.strings[1]);
}

void test_abf_strcache_expands_when_needed(void) {
    char *str1 = "one";
    int i;
    for (i=0; i<10; i++) {
        abf_strcache_add(&sc, str1);
    }
    char *str2 = "two";
    abf_strcache_add(&sc, str2);
    TEST_ASSERT_EQUAL_STRING(str1, sc.strings[i-1]);
    TEST_ASSERT_EQUAL_STRING(str2, sc.strings[i]);
}

void test_abf_strcache_get_retrieves_string(void) {
    char *str1 = "A string cache string";
    char *str2 = "Another string";
    abf_strcache_add(&sc, str1);
    abf_strcache_add(&sc, str2);
    TEST_ASSERT_EQUAL_STRING(str1, abf_strcache_get(&sc, 0));
    TEST_ASSERT_EQUAL_STRING(str2, abf_strcache_get(&sc, 1));
}

void test_abf_strcache_get_returns_null_if_no_string(void) {
    char *str1 = "A string cache string";
    abf_strcache_add(&sc, str1);
    TEST_ASSERT_NULL(abf_strcache_get(&sc, 1));
}

void test_abf_strcache_remove_deletes_string(void) {
    char *str1 = "one";
    char *str2 = "two";
    char *str3 = "three";
    abf_strcache_add(&sc, str1);
    abf_strcache_add(&sc, str2);
    abf_strcache_add(&sc, str3);

    abf_strcache_remove(&sc, 1);
    TEST_ASSERT_EQUAL_INT(2, sc.count);
    TEST_ASSERT_EQUAL_STRING(str3, abf_strcache_get(&sc, 1));
}
