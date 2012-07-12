#ifndef _LLIST_H_
#define _LLIST_H_

#include <stddef.h>

/* Circular doubly-linked list implementation in standard C based on
 * Linux <linux/list.h> implementation (which is in GNU C) */

/**
 * list entry type
 *
 * To use, add a member of this type to the desired structure. for instance,
 *
 * struct foo {
 *   int bar;
 *   struct list_head list;
 * }
 */
struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

/**
 * LIST_HEAD_INIT - initialize list head statically at compile-time
 */
#define LIST_HEAD_INIT(name) { &(name), &(name) }

/**
 * LIST_HEAD - define a pointer to a list that is not a list entry
 */
#define LIST_HEAD(name)                         \
    struct list_head name = LIST_HEAD_INIT(name)

/**
 * INIT_LIST_HEAD - initialize list head at run-time
 */
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

/**
 * container_of - get pointer to containing struct of struct member
 * @ptr:    a pointer to the struct member
 * @type:   the type of struct
 * @member: the name of the struct member
 *
 * note: this macro does no type-checking
 * (implementation from AndreyT: http://stackoverflow.com/a/6132697/192780)
 */
#define container_of(ptr, type, member) (               \
        (type *) ((char*)ptr - offsetof(type, member)))

/**
 * list_entry - get the struct for this list entry
 * @ptr:    pointer to the struct list_head member
 * @type:   the type of struct the entry is embedded in
 * @member: the name of the list_head member within the struct
 */
#define list_entry(ptr, type, member)           \
    container_of(ptr, type, member)

#endif /* _LLIST_H_ */
