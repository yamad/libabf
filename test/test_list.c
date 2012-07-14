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

struct hare {
    unsigned long size;
    unsigned long weight;
    bool is_fast;
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

void test_LIST_HEAD_creates_and_inits_list_var(void)
{
    LIST_HEAD(test_list);
    TEST_ASSERT_EQUAL_PTR(&test_list, test_list.next);
    TEST_ASSERT_EQUAL_PTR(&test_list, test_list.prev);
}

void test_list_add_single_entry(void)
{
    LIST_HEAD(test_list);
    struct fox foxA = {
        .is_fantastic = true,
        .list = LIST_HEAD_INIT(foxA.list)
    };
    list_add(&foxA.list, &test_list);
    TEST_ASSERT_EQUAL_PTR(&foxA.list, test_list.next);
    TEST_ASSERT_EQUAL_PTR(&foxA.list, test_list.prev);
}

void test_list_add_multiple_entries(void)
{
    LIST_HEAD(test_list);
    struct fox foxA = {
        .is_fantastic = true,
        .list = LIST_HEAD_INIT(foxA.list)
    };
    struct fox foxB = {
        .is_fantastic = true,
        .list = LIST_HEAD_INIT(foxB.list)
    };

    /* create links: test_list -> foxA -> foxB */
    list_add(&foxB.list, &test_list);
    list_add(&foxA.list, &test_list);

    /* test forward pointers */
    TEST_ASSERT_EQUAL_PTR(&foxA.list, test_list.next);
    TEST_ASSERT_EQUAL_PTR(&foxB.list, foxA.list.next);
    TEST_ASSERT_EQUAL_PTR(&test_list, foxB.list.next);

    /* test backward pointers */
    TEST_ASSERT_EQUAL_PTR(&foxB.list, test_list.prev);
    TEST_ASSERT_EQUAL_PTR(&foxA.list, foxB.list.prev);
    TEST_ASSERT_EQUAL_PTR(&test_list, foxA.list.prev);
}

void test_struct_with_list_member(void)
{
    struct list_of_lists {
        struct list_head fox_list;
        struct list_head hare_list;
    };

    struct list_of_lists lol = {
        .fox_list = LIST_HEAD_INIT(lol.fox_list),
        .hare_list = LIST_HEAD_INIT(lol.hare_list)
    };

    struct fox a_fox = {
        .is_fantastic = true,
        .list = LIST_HEAD_INIT(a_fox.list)
    };

    struct hare a_hare = {
        .is_fast = false,
        .list = LIST_HEAD_INIT(a_hare.list)
    };

    list_add(&a_fox.list, &lol.fox_list);
    list_add(&a_hare.list, &lol.hare_list);
}
