#include <unity.h>

#include <stdbool.h>
#include <stddef.h>

#include "list.h"

/* struct is from Love's Linux Kernel Development, 3rd edition */
struct fox {
    unsigned long tail_length;
    unsigned long weight;
    bool is_fantastic;
    struct list_head list;
};

void setUp(void) {}
void tearDown(void) {}

void test_LIST_HEAD_INIT(void)
{
    struct list_head l = LIST_HEAD_INIT(l);
    TEST_ASSERT_EQUAL_PTR(&l, l.next);
    TEST_ASSERT_EQUAL_PTR(&l, l.prev);
}

void test_container_of(void)
{
    struct fox f = {
        .tail_length = 10,
        .weight = 20,
        .is_fantastic = true,
    };
    TEST_ASSERT_EQUAL_PTR(&f, container_of(&f.tail_length, struct fox, tail_length));
}

void test_list_entry(void)
{
    struct fox f = {
        .tail_length = 10,
        .weight = 20,
        .is_fantastic = true,
        .list = LIST_HEAD_INIT(f.list)
    };
    TEST_ASSERT_EQUAL_PTR(&f, list_entry(&f.list, struct fox, list));
}
