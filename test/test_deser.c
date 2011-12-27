#include "unity.h"
#include "deserialize.h"

void setUp(void) {}
void tearDown(void) {}

void test_floatrep(void)
{
    char buf[4] = { 0x3E, 0x20, 0x00, 0x00 };
    float f1 = (float)0x3E200000;
    TEST_ASSERT_EQUAL_FLOAT(0x3E210000, f1);
}

void test_read_char_gets_char_at_offset(void)
{
    char buf[2] = { 0xCA, 0xFE };
    char b1 = read_char(buf, 0);
    char b2 = read_char(buf, 1);
    TEST_ASSERT_EQUAL_HEX8(0xCA, b1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b2);
}

void test_read_uint8_get_uint8(void)
{
    char buf[2] = { 0xCA, 0xFE };
    uint8_t b1 = read_uint8(buf, 1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b1);
}

void test_read_int8_get_int8(void)
{
    char buf[2] = { 0xCA, 0xFE };
    int8_t b1 = read_int8(buf, 1);
    TEST_ASSERT_EQUAL_INT8(0xFE, b1);
}

void test_read_uint16_gets_uint16(void)
{
    char buf[2] = { 0xCA, 0xFE };
    bool swap = 0;
    uint16_t result = read_uint16(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
}

void test_read_uint16_gets_swapped_uint16(void)
{
    char buf[2] = { 0xCA, 0xFE };
    bool swap = 1;
    uint16_t result = read_uint16(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
}

void test_read_int16_gets_int16(void)
{
    char buf[2] = { 0xCA, 0xFE };
    bool swap = 0;
    int16_t result = read_int16(buf, 0, swap);
    TEST_ASSERT_EQUAL_INT16(0xCAFE, result);
}

void test_read_int16_gets_swapped_int16(void)
{
    char buf[2] = { 0xCA, 0xFE };
    bool swap = 1;
    int16_t result = read_int16(buf, 0, swap);
    TEST_ASSERT_EQUAL_INT16(0xFECA, result);
}

void test_read_uint32_gets_uint32(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    uint32_t result = read_uint32(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
}

void test_read_uint32_gets_swapped_uint32(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 1;
    uint32_t result = read_uint32(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
}

void test_read_int32_gets_int32(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    int32_t result = read_int32(buf, 0, swap);
    TEST_ASSERT_EQUAL_INT32(0xCAFEBEEF, result);
}

void test_read_int32_gets_swapped_int32(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 1;
    int32_t result = read_int32(buf, 0, swap);
    TEST_ASSERT_EQUAL_INT32(0xEFBEFECA, result);
}

void test_read_uint64_gets_uint64(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    bool swap = 0;
    uint64_t result = read_uint64(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
}

void test_read_uint64_gets_swapped_uint64(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    bool swap = 1;
    uint64_t result = read_uint64(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
}

void test_read_int64_gets_int64(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    bool swap = 0;
    int64_t result = read_int64(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
}

void test_read_int64_gets_swapped_int64(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    bool swap = 1;
    int64_t result = read_int64(buf, 0, swap);
    TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
}

void test_read_charp_advances_char_pointer(void) 
{
    char buf[2] = { 0xCA, 0xFE };
    const char *bufp = buf;
    char b1 = read_charp(&bufp);
    char b2 = read_charp(&bufp);
    TEST_ASSERT_EQUAL_HEX8(0xCA, b1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b2);
}

void test_read_uint8p_reads_uint8s_inorder(void)
{
    char buf[2] = { 0xCA, 0xFE };
    const char *bufp = buf;
    uint8_t b1 = read_uint8p(&bufp);
    uint8_t b2 = read_uint8p(&bufp);
    TEST_ASSERT_EQUAL_HEX8(0xCA, b1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b2);
}

void test_read_int8p_reads_int8(void)
{
    char buf[2] = { 0xCA, 0xFE };
    const char *bufp = buf;
    int8_t b1 = read_int8p(&bufp);
    int8_t b2 = read_int8p(&bufp);
    TEST_ASSERT_EQUAL_HEX8(0xCA, b1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b2);
}

void test_read_uint16p_reads_uint16s_inorder(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    const char *bufp = buf;
    bool swap = 0;
    uint16_t i1 = read_uint16p(&bufp, swap);
    uint16_t i2 = read_uint16p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX16(0xCAFE, i1);
    TEST_ASSERT_EQUAL_HEX16(0xBEEF, i2);
}

void test_read_uint16p_reads_swapped_uint16s(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    const char *bufp = buf;
    bool swap = 1;
    uint16_t i1 = read_uint16p(&bufp, swap);
    uint16_t i2 = read_uint16p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX16(0xFECA, i1);
    TEST_ASSERT_EQUAL_HEX16(0xEFBE, i2);
}

void test_read_int16p_reads_int16(void)
{
    char buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    const char *bufp = buf;
    bool swap = 0;
    int16_t i1 = read_int16p(&bufp, swap);
    int16_t i2 = read_int16p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX16(0xCAFE, i1);
    TEST_ASSERT_EQUAL_HEX16(0xBEEF, i2);
}

void test_read_uint32p_reads_uint32s_inorder(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    const char *bufp = buf;
    bool swap = 0;
    uint32_t i1 = read_uint32p(&bufp, swap);
    uint32_t i2 = read_uint32p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, i1);
    TEST_ASSERT_EQUAL_HEX32(0xBAADCAAF, i2);
}

void test_read_uint32p_reads_swapped_uint32s(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    const char *bufp = buf;
    bool swap = 1;
    uint32_t i1 = read_uint32p(&bufp, swap);
    uint32_t i2 = read_uint32p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, i1);
    TEST_ASSERT_EQUAL_HEX32(0xAFCAADBA, i2);
}

void test_read_int32p_reads_int32(void)
{
    char buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF};
    const char *bufp = buf;
    bool swap = 0;
    int32_t i1 = read_int32p(&bufp, swap);
    int32_t i2 = read_int32p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, i1);
    TEST_ASSERT_EQUAL_HEX32(0xBAADCAAF, i2);
}

void test_read_uint64p_reads_uint64s_inorder(void)
{
    char buf[16] = {
        0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF,        \
        0x07, 0x03, 0x04, 0x06, 0x08, 0x14, 0x20, 0x54         \
    };
    const char *bufp = buf;
    bool swap = 0;
    uint64_t i1 = read_uint64p(&bufp, swap);
    uint64_t i2 = read_uint64p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, i1);
    TEST_ASSERT_EQUAL_HEX64(0x0703040608142054, i2);
}

void test_read_uint64p_reads_swapped_uint64s(void)
{
    char buf[16] = {
        0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF,        \
        0x07, 0x03, 0x04, 0x06, 0x08, 0x14, 0x20, 0x54         \
    };
    const char *bufp = buf;
    bool swap = 1;
    uint64_t i1 = read_uint64p(&bufp, swap);
    uint64_t i2 = read_uint64p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, i1);
    TEST_ASSERT_EQUAL_HEX64(0x5420140806040307, i2);
}

void test_read_int64p_reads_int64(void)
{
    char buf[16] = {
        0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF,        \
        0x07, 0x03, 0x04, 0x06, 0x08, 0x14, 0x20, 0x54         \
    };
    const char *bufp = buf;
    bool swap = 0;
    int64_t i1 = read_int64p(&bufp, swap);
    int64_t i2 = read_int64p(&bufp, swap);
    TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, i1);
    TEST_ASSERT_EQUAL_HEX64(0x0703040608142054, i2);
}
