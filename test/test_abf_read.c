#include <stdlib.h>
#include "unity.h"

#include "deserialize.h"
#include "swap.h"
#include "abf_read.h"

void setUp(void) {}
void tearDown(void) {}

void test_abf_read_logfont(void)
{
    uint8_t bytes[ABF_LOGFONTSIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_logfont linfo;
    buf = abf_read_logfont(buf, &linfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_LOGFONTSIZE, buf - head);
}

void test_abf_read_signal(void)
{
    uint8_t bytes[ABF_SIGNALSIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_signal sinfo;
    buf = abf_read_signal(buf, &sinfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_SIGNALSIZE, buf - head);
}

void test_abf_read_scopeconfig(void)
{
    uint8_t bytes[ABF_SCOPECONFIGSIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_scopeconfig sinfo;
    buf = abf_read_scopeconfig(buf, &sinfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_SCOPECONFIGSIZE, buf - head);
}

void test_abf_read_tag(void)
{
    uint8_t bytes[ABF_TAGSIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_tag tinfo;
    buf = abf_read_tag(buf, &tinfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_TAGSIZE, buf - head);
}

void test_abf_read_synch(void)
{
    uint8_t bytes[ABF_SYNCHSIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_synch sinfo;
    buf = abf_read_synch(buf, &sinfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_SYNCHSIZE, buf - head);
}

void test_abf_read_voicetaginfo(void)
{
    uint8_t bytes[ABF_VOICETAGINFOSIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_voicetaginfo vinfo;
    buf = abf_read_voicetaginfo(buf, &vinfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_VOICETAGINFOSIZE, buf - head);
}

void test_abf_read_delta(void)
{
    uint8_t bytes[ABF_DELTASIZE];
    uint8_t *buf = bytes;
    uint8_t *head = buf;

    bool to_swap;
    if (ENDIAN_LITTLE == get_endian()) {
        to_swap = false;
    } else {
        to_swap = true;
    }
    struct abf_delta dinfo;
    buf = abf_read_delta(buf, &dinfo, to_swap);
    TEST_ASSERT_EQUAL_INT(ABF_DELTASIZE, buf - head);
}
