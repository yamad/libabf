#include <stdlib.h>
#include "unity.h"

#include "abf2.h"

void setUp(void) {}
void tearDown(void) {}

void test_abf2_read_section(void) {
    char buf[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0X09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    char *bufp = buf;
    bool to_swap = 0;

    struct abf2_section *sec;
    sec = (struct abf2_section *)malloc(sizeof(struct abf2_section));
    abf2_read_section(&bufp, &sec, to_swap);
}
