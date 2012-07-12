#include <stdlib.h>
#include "unity.h"

#include "deserialize.h"
#include "swap.h"
#include "abf_read.h"

void setUp(void) {}
void tearDown(void) {}

void test_abf_read_logfont(void)
{
    char bytes[ABF_LOGFONTSIZE];
    char *buf = bytes;
    char *head = buf;

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
    char bytes[ABF_SIGNALSIZE];
    char *buf = bytes;
    char *head = buf;

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
    char bytes[ABF_SCOPECONFIGSIZE];
    char *buf = bytes;
    char *head = buf;

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
    char bytes[ABF_TAGSIZE];
    char *buf = bytes;
    char *head = buf;

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
    char bytes[ABF_SYNCHSIZE];
    char *buf = bytes;
    char *head = buf;

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
    char bytes[ABF_VOICETAGINFOSIZE];
    char *buf = bytes;
    char *head = buf;

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
    char bytes[ABF_DELTASIZE];
    char *buf = bytes;
    char *head = buf;

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
