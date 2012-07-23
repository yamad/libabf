#include <stdlib.h>
#include <string.h>


#include "unity.h"
#include "deserialize.h"
#include "swap.h"

void setUp(void) {}
void tearDown(void) {}

void test_read_uint8_get_uint8(void)
{
    uint8_t buf[2] = { 0xCA, 0xFE };
    uint8_t b1 = read_uint8(buf, 1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b1);
}

void test_read_int8_get_int8(void)
{
    uint8_t buf[2] = { 0xCA, 0xFE };
    int8_t b1 = read_int8(buf, 1);
    TEST_ASSERT_EQUAL_INT8(0xFE, b1);
}

void test_read_uint16_gets_uint16(void)
{
    uint8_t buf[2] = { 0xCA, 0xFE };
    bool swap = 0;
    uint16_t result = read_uint16(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    }
}

void test_read_uint16_gets_uint16_at_offset(void)
{
    uint8_t buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    uint16_t u1 = read_uint16(buf, 2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xEFBE, u1);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xBEEF, u1);
    }
}

void test_read_uint16_gets_swapped_uint16(void)
{
    uint8_t buf[2] = { 0xCA, 0xFE };
    bool swap = 1;
    uint16_t result = read_uint16(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, result);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, result);
    }
}

void test_read_int16_gets_int16(void)
{
    uint8_t buf[2] = { 0xCA, 0xFE };
    bool swap = 0;
    int16_t result = read_int16(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_INT16(0xFECA, result);
    } else {
        TEST_ASSERT_EQUAL_INT16(0xCAFE, result);
    }
}

void test_read_int16_gets_int16_at_offset(void)
{
    uint8_t buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    int16_t i1 = read_uint16(buf, 2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xEFBE, i1);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xBEEF, i1);
    }
}

void test_read_int16_gets_swapped_int16(void)
{
    uint8_t buf[2] = { 0xCA, 0xFE };
    bool swap = 1;
    int16_t result = read_int16(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_INT16(0xCAFE, result);
    } else {
        TEST_ASSERT_EQUAL_INT16(0xFECA, result);
    }
}

void test_read_uint32_gets_uint32(void)
{
    uint8_t buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    uint32_t result = read_uint32(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    }
}

void test_read_uint32_gets_uint32_at_offset(void)
{
    uint8_t buf[6] = { 0x00, 0x00, 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    uint32_t u1 = read_uint32(buf, 2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, u1);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, u1);
    }
}

void test_read_uint32_gets_swapped_uint32(void)
{
    uint8_t buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 1;
    uint32_t result = read_uint32(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    }
}

void test_read_int32_gets_int32(void)
{
    uint8_t buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    int32_t result = read_int32(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    }
}

void test_read_int32_gets_int32_at_offset(void)
{
    uint8_t buf[6] = { 0x00, 0x00, 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 0;
    int32_t i1 = read_int32(buf, 2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, i1);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, i1);
    }
}

void test_read_int32_gets_swapped_int32(void)
{
    uint8_t buf[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    bool swap = 1;
    int32_t result = read_int32(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, result);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, result);
    }
}

void test_read_uint64_gets_uint64(void)
{
    uint8_t buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    bool swap = 0;
    uint64_t result = read_uint64(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
    }
}

void test_read_uint64_gets_uint64_at_offset(void)
{
    uint8_t buf[9] = { 0x00, 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    bool swap = 0;
    uint64_t result = read_uint64(buf, 1, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
    }
}

void test_read_uint64_gets_swapped_uint64(void)
{
    uint8_t buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    bool swap = 1;
    uint64_t result = read_uint64(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
    }
}

void test_read_int64_gets_int64(void)
{
    uint8_t buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    bool swap = 0;
    int64_t result = read_int64(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
    }
}

void test_read_int64_gets_int64_at_offset(void)
{
    uint8_t buf[9] = { 0x00, 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    bool swap = 0;
    int64_t result = read_int64(buf, 1, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
    }
}

void test_read_int64_gets_swapped_int64(void)
{
    uint8_t buf[8] = { 0xCA, 0xFE, 0xBE, 0xEF , 0xBA, 0xAD, 0xCA, 0xAF };
    bool swap = 1;
    int64_t result = read_int64(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, result);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, result);
    }
}

void test_read_float32_get_float(void)
{
    uint8_t buf[4] = { 0x66, 0xA6, 0xE6, 0x3F };
    bool swap = false;
    float f = read_float32(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_FLOAT(1.801953, f);
    } else {
        TEST_ASSERT_EQUAL_FLOAT(394080063460869345902592.0, f);
    }
}

void test_read_float32_get_swapped_float(void)
{
    uint8_t buf[4] = { 0x66, 0xA6, 0xE6, 0x3F };
    bool swap = true;
    float f = read_float32(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_FLOAT(394080063460869345902592.0, f);
    } else {
        TEST_ASSERT_EQUAL_FLOAT(1.801953, f);
    }
}

void test_read_float64_get_double(void)
{
    uint8_t buf[8] = { 0x7A, 0x14, 0xAE, 0x47, 0xE1, 0x7A, 0x84, 0x7F };
    bool swap = false;
    double d = read_float64(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_DOUBLE(1797693134862315633301262629765050630866728936377003738459752501701914801454763796210449641241516314779480883561305033792001456611676070178718250902275612892141556670084334558762242165238180420033094130920939320073544954448635079731752693526288593899415905695239791325170347763968341437224060469123940876288.000000, d);
    } else {
        TEST_ASSERT_EQUAL_DOUBLE(11731214887579692300750387334399227980730006290306971769102461373710196481992665851316629523374336388817328033953416558476377592188969935274886220748934030397222706304474864884233531207106413830435199508403584936941314263743467511926564159735259568285560827574331855785640668954624.000000, d);
    }
}

void test_read_float64_get_swapped_double(void)
{
    uint8_t buf[8] = { 0x7A, 0x14, 0xAE, 0x47, 0xE1, 0x7A, 0x84, 0x7F };
    bool swap = true;
    double d = read_float64(buf, 0, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_DOUBLE(11731214887579692300750387334399227980730006290306971769102461373710196481992665851316629523374336388817328033953416558476377592188969935274886220748934030397222706304474864884233531207106413830435199508403584936941314263743467511926564159735259568285560827574331855785640668954624.000000, d);
    } else {
        TEST_ASSERT_EQUAL_DOUBLE(1797693134862315633301262629765050630866728936377003738459752501701914801454763796210449641241516314779480883561305033792001456611676070178718250902275612892141556670084334558762242165238180420033094130920939320073544954448635079731752693526288593899415905695239791325170347763968341437224060469123940876288.000000, d);
    }
}

void test_readn_uint8p_reads_n_bytes(void)
{
    uint8_t bytes[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    uint8_t *buf = bytes;
    uint8_t *head = buf;
    uint8_t res[2];
    buf = readn_uint8p(buf, res, 2);
    TEST_ASSERT_EQUAL_INT(2, buf - head); /* pointer advances */
    TEST_ASSERT_EQUAL_HEX8(0xCA, res[0]); /* can get data */
    TEST_ASSERT_EQUAL_HEX8(0xFE, res[1]);
    TEST_ASSERT_EQUAL_HEX8(0xBE, *buf); /* pointer points to unconsumed data */
}

void test_read_uint8p_reads_uint8s_inorder(void)
{
    uint8_t bytes[2] = { 0xCA, 0xFE };
    uint8_t *buf = bytes;
    uint8_t b1, b2;
    buf = read_uint8p(buf, &b1);
    buf = read_uint8p(buf, &b2);
    TEST_ASSERT_EQUAL_HEX8(0xCA, b1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b2);
}

void test_read_int8p_reads_int8(void)
{
    uint8_t bytes[2] = { 0xCA, 0xFE };
    uint8_t *buf = bytes;
    int8_t b1, b2;
    buf = read_int8p(buf, &b1);
    buf = read_int8p(buf, &b2);
    TEST_ASSERT_EQUAL_HEX8(0xCA, b1);
    TEST_ASSERT_EQUAL_HEX8(0xFE, b2);
}

void test_read_uint16p_reads_uint16s_inorder(void)
{
    uint8_t bytes[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    uint8_t *buf = bytes;
    uint16_t i1, i2;
    bool swap = 0;
    buf = read_uint16p(buf, &i1, swap);
    buf = read_uint16p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, i1);
        TEST_ASSERT_EQUAL_HEX16(0xEFBE, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, i1);
        TEST_ASSERT_EQUAL_HEX16(0xBEEF, i2);
    }
}

void test_read_uint16p_reads_swapped_uint16s(void)
{
    uint8_t bytes[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    uint8_t *buf = bytes;
    uint16_t i1, i2;
    bool swap = 1;
    buf = read_uint16p(buf, &i1, swap);
    buf = read_uint16p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, i1);
        TEST_ASSERT_EQUAL_HEX16(0xBEEF, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, i1);
        TEST_ASSERT_EQUAL_HEX16(0xEFBE, i2);
    }
}

void test_read_int16p_reads_int16(void)
{
    uint8_t bytes[4] = { 0xCA, 0xFE, 0xBE, 0xEF };
    uint8_t *buf = bytes;
    int16_t i1, i2;
    bool swap = 0;
    buf = read_int16p(buf, &i1, swap);
    buf = read_int16p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX16(0xFECA, i1);
        TEST_ASSERT_EQUAL_HEX16(0xEFBE, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX16(0xCAFE, i1);
        TEST_ASSERT_EQUAL_HEX16(0xBEEF, i2);
    }
}

void test_read_uint32p_reads_uint32s_inorder(void)
{
    uint8_t bytes[8] = { 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    uint8_t *buf = bytes;
    uint32_t i1, i2;
    bool swap = 0;
    buf = read_uint32p(buf, &i1, swap);
    buf = read_uint32p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, i1);
        TEST_ASSERT_EQUAL_HEX32(0xAFCAADBA, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, i1);
        TEST_ASSERT_EQUAL_HEX32(0xBAADCAAF, i2);
    }
}

void test_read_uint32p_reads_swapped_uint32s(void)
{
    uint8_t bytes[8] = { 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    uint8_t *buf = bytes;
    uint32_t i1, i2;
    bool swap = 1;
    buf = read_uint32p(buf, &i1, swap);
    buf = read_uint32p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, i1);
        TEST_ASSERT_EQUAL_HEX32(0xBAADCAAF, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, i1);
        TEST_ASSERT_EQUAL_HEX32(0xAFCAADBA, i2);
    }
}

void test_read_int32p_reads_int32(void)
{
    uint8_t bytes[8] = { 0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF };
    uint8_t *buf = bytes;
    int32_t i1, i2;
    bool swap = 0;
    buf = read_int32p(buf, &i1, swap);
    buf = read_int32p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX32(0xEFBEFECA, i1);
        TEST_ASSERT_EQUAL_HEX32(0xAFCAADBA, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX32(0xCAFEBEEF, i1);
        TEST_ASSERT_EQUAL_HEX32(0xBAADCAAF, i2);
    }
}

void test_read_uint64p_reads_uint64s_inorder(void)
{
    uint8_t bytes[16] = {
        0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF,
        0x07, 0x03, 0x04, 0x06, 0x08, 0x14, 0x20, 0x54
    };
    uint8_t *buf = bytes;
    uint64_t i1, i2;
    bool swap = 0;
    buf = read_uint64p(buf, &i1, swap);
    buf = read_uint64p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, i1);
        TEST_ASSERT_EQUAL_HEX64(0x5420140806040307, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, i1);
        TEST_ASSERT_EQUAL_HEX64(0x0703040608142054, i2);
    }
}

void test_read_uint64p_reads_swapped_uint64s(void)
{
    uint8_t bytes[16] = {
        0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF,
        0x07, 0x03, 0x04, 0x06, 0x08, 0x14, 0x20, 0x54
    };
    uint8_t *buf = bytes;
    uint64_t i1, i2;
    bool swap = 1;
    buf = read_uint64p(buf, &i1, swap);
    buf = read_uint64p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, i1);
        TEST_ASSERT_EQUAL_HEX64(0x0703040608142054, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, i1);
        TEST_ASSERT_EQUAL_HEX64(0x5420140806040307, i2);
    }
}

void test_read_int64p_reads_int64(void)
{
    uint8_t bytes[16] = {
        0xCA, 0xFE, 0xBE, 0xEF, 0xBA, 0xAD, 0xCA, 0xAF,
        0x07, 0x03, 0x04, 0x06, 0x08, 0x14, 0x20, 0x54
    };
    uint8_t *buf = bytes;
    int64_t i1, i2;
    bool swap = 0;
    buf = read_int64p(buf, &i1, swap);
    buf = read_int64p(buf, &i2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_HEX64(0xAFCAADBAEFBEFECA, i1);
        TEST_ASSERT_EQUAL_HEX64(0x5420140806040307, i2);
    } else {
        TEST_ASSERT_EQUAL_HEX64(0xCAFEBEEFBAADCAAF, i1);
        TEST_ASSERT_EQUAL_HEX64(0x0703040608142054, i2);
    }
}

void test_read_float32p_reads_single_floats(void)
{
    uint8_t bytes[8] = {
        0x66, 0xA6, 0xE6, 0x3F,
        0x3F, 0xE6, 0xA6, 0x66
    };
    uint8_t *buf = bytes;
    float f1, f2;
    bool swap = false;
    buf = read_float32p(buf, &f1, swap);
    buf = read_float32p(buf, &f2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_FLOAT(1.801953, f1);
        TEST_ASSERT_EQUAL_FLOAT(394080063460869345902592.0, f2);
    } else {
        TEST_ASSERT_EQUAL_FLOAT(394080063460869345902592.0, f1);
        TEST_ASSERT_EQUAL_FLOAT(1.801953, f2);
    }
}

void test_read_float32p_reads_swapped_floats(void)
{
    uint8_t bytes[8] = {
        0x66, 0xA6, 0xE6, 0x3F,
        0x3F, 0xE6, 0xA6, 0x66
    };
    uint8_t *buf = bytes;
    float f1, f2;
    bool swap = true;
    buf = read_float32p(buf, &f1, swap);
    buf = read_float32p(buf, &f2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_FLOAT(394080063460869345902592.0, f1);
        TEST_ASSERT_EQUAL_FLOAT(1.801953, f2);
    } else {
        TEST_ASSERT_EQUAL_FLOAT(1.801953, f1);
        TEST_ASSERT_EQUAL_FLOAT(394080063460869345902592.0, f2);
    }
}

void test_read_float64p_reads_double_floats(void)
{
    uint8_t bytes[16] = {
        0x7A, 0x14, 0xAE, 0x47, 0xE1, 0x7A, 0x84, 0x7F,
        0x7F, 0x84, 0x7A, 0xE1, 0x47, 0xAE, 0x14, 0x7A
    };
    uint8_t *buf = bytes;
    double f1, f2;
    bool swap = false;
    buf = read_float64p(buf, &f1, swap);
    buf = read_float64p(buf, &f2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_DOUBLE(1797693134862315633301262629765050630866728936377003738459752501701914801454763796210449641241516314779480883561305033792001456611676070178718250902275612892141556670084334558762242165238180420033094130920939320073544954448635079731752693526288593899415905695239791325170347763968341437224060469123940876288.000000, f1);
        TEST_ASSERT_EQUAL_DOUBLE(11731214887579692300750387334399227980730006290306971769102461373710196481992665851316629523374336388817328033953416558476377592188969935274886220748934030397222706304474864884233531207106413830435199508403584936941314263743467511926564159735259568285560827574331855785640668954624.000000, f2);
    } else {
        TEST_ASSERT_EQUAL_DOUBLE(11731214887579692300750387334399227980730006290306971769102461373710196481992665851316629523374336388817328033953416558476377592188969935274886220748934030397222706304474864884233531207106413830435199508403584936941314263743467511926564159735259568285560827574331855785640668954624.000000, f1);
        TEST_ASSERT_EQUAL_DOUBLE(1797693134862315633301262629765050630866728936377003738459752501701914801454763796210449641241516314779480883561305033792001456611676070178718250902275612892141556670084334558762242165238180420033094130920939320073544954448635079731752693526288593899415905695239791325170347763968341437224060469123940876288.000000, f2);
    }
}

void test_read_float64p_reads_swapped_double_floats(void)
{
    uint8_t bytes[16] = {
        0x7A, 0x14, 0xAE, 0x47, 0xE1, 0x7A, 0x84, 0x7F,
        0x7F, 0x84, 0x7A, 0xE1, 0x47, 0xAE, 0x14, 0x7A
    };
    uint8_t *buf = bytes;
    double f1, f2;
    bool swap = true;
    buf = read_float64p(buf, &f1, swap);
    buf = read_float64p(buf, &f2, swap);
    if (ENDIAN_LITTLE == get_endian()) {
        TEST_ASSERT_EQUAL_DOUBLE(11731214887579692300750387334399227980730006290306971769102461373710196481992665851316629523374336388817328033953416558476377592188969935274886220748934030397222706304474864884233531207106413830435199508403584936941314263743467511926564159735259568285560827574331855785640668954624.000000, f1);
        TEST_ASSERT_EQUAL_DOUBLE(1797693134862315633301262629765050630866728936377003738459752501701914801454763796210449641241516314779480883561305033792001456611676070178718250902275612892141556670084334558762242165238180420033094130920939320073544954448635079731752693526288593899415905695239791325170347763968341437224060469123940876288.000000, f2);
    } else {
        TEST_ASSERT_EQUAL_DOUBLE(1797693134862315633301262629765050630866728936377003738459752501701914801454763796210449641241516314779480883561305033792001456611676070178718250902275612892141556670084334558762242165238180420033094130920939320073544954448635079731752693526288593899415905695239791325170347763968341437224060469123940876288.000000, f1);
        TEST_ASSERT_EQUAL_DOUBLE(11731214887579692300750387334399227980730006290306971769102461373710196481992665851316629523374336388817328033953416558476377592188969935274886220748934030397222706304474864884233531207106413830435199508403584936941314263743467511926564159735259568285560827574331855785640668954624.000000, f2);
    }
}

void test_read_string_gets_valid_string(void) {
    uint8_t bytes[10] = { 0x63,0x6C,0x61,0x6D,0x70,0x65,0x78,0x00,0x43,0x3A };
    size_t MAX_STRING_SIZE = 100;
    size_t bytes_consumed;
    char *str1 = read_string(bytes, MAX_STRING_SIZE, &bytes_consumed);
    TEST_ASSERT_EQUAL_STRING("clampex", str1);
    TEST_ASSERT_EQUAL(*(str1+8), '\0');
    TEST_ASSERT_EQUAL(8, bytes_consumed);
}

void test_read_string_truncates_at_nmax_chars(void) {
    uint8_t bytes[8] = { 0x63,0x6C,0x61,0x6D,0x70,0x65,0x78,0x00 };
    size_t bytes_consumed;
    char *str1 = read_string(bytes, 4, &bytes_consumed);
    TEST_ASSERT_EQUAL_STRING("clam", str1);
    TEST_ASSERT_EQUAL(*(str1+4), '\0');
    TEST_ASSERT_EQUAL(4, bytes_consumed);
}

void test_read_stringp_gets_valid_string(void) {
    uint8_t bytes[10] = { 0x63,0x6C,0x61,0x6D,0x70,0x65,0x78,0x00,0x43,0x3A };
    uint8_t *buf = bytes;
    size_t MAX_STRING_SIZE = 100;

    char *str1;
    buf = read_stringp(buf, &str1, MAX_STRING_SIZE);
    TEST_ASSERT_EQUAL_STRING("clampex", str1);
}

void test_read_stringp_advances_buf_pointer(void) {
    uint8_t bytes[10] = { 0x63,0x6C,0x61,0x6D,0x70,0x65,0x78,0x00,0x43,0x3A };
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    size_t MAX_STRING_SIZE = 100;

    char *str1;
    buf = read_stringp(buf, &str1, MAX_STRING_SIZE);

    /* should advance past null terminator */
    size_t len = strlen(str1);
    TEST_ASSERT_EQUAL_INT(len+1, buf - head);

    uint8_t b = read_uint8(buf, 0);
    TEST_ASSERT_EQUAL_HEX8(0x43, b);
}
