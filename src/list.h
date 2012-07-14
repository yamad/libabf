#ifndef _LLIST_H_
#define _LLIST_H_

#include <stddef.h>

/* Doubly-linked list implementation in standard C. Modified from
 * Linux <linux/list.h> for general use.
 *
 * List nodes are stored inside of parent structures, which allows for
 * a completely general implementation of lists. The data "payload" of
 * each list entry is obtained by the `list_entry` macro which returns
 * the containing struct of a list node. This design is copied from
 * the design of linked lists in the Linux kernel.
 *
 *
 * Note that the implementation is inverted with respect to the usual
 * design of lists in which the data is stored within a list node. For
 * instance, a common idiom is::
 *
 * struct foo {
 *   int member_bar;
 *   int member_baz;
 *   struct foo *next;
 *   struct foo *prev;
 * };
 *
 * or for more generality,
 *
 * struct list_node {
 *   void *data;
 *   struct list_node *next;
 *   struct list_node *prev;
 * };
 *
 * This implementation does neither of the above and instead opts for
 * storing the list inside the data::
 *
 * struct foo {
 *    int member_bar;
 *    int member_baz;
 *    struct list_head list;
 * };
 *
 * In particular, this implementation (i) generalizes list handling,
 * (ii) eases structure allocation, and (iii) cleanly handles
 * including a structure in multiple lists.
 *
 * Changes from linux/list.h:
 *
 *   * remove "inline" declaration specifiers for C89 compatibility
 *   * change implementation of `container_of` to use only ANSI C
 *   * add `type` member to macros that depended on `typeof` (typeof is
 *     a GCC extension)
 *   * only functions that are used are implemented
 */

/**
 * list entry type
 *
 * Add a member of this type to make a linked-list out of a
 * struct. For instance,
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
static void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

/**
 * insert a new entry between two known consecutive entries.
 *
 * only use if the next/prev entries are already known
 */
static void __list_add(struct list_head *new,
                       struct list_head *prev,
                       struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
 * remove entry from list by making prev/next entries point to each
 * other.
 */
static void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * remove entry from list. entry is in undefined state.
 */
static void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

/**
 * list_add - add a new entry after specified head
 * @new:  entry to add
 * @head: list entry to insert after
 */
static void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry before specified head
 * @new:  entry to add
 * @head: list entry to insert before
 */
static void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static void list_replace(struct list_head *old,
                         struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static void list_replace_init(struct list_head *old,
                              struct list_head *new)
{
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static int list_is_last(const struct list_head *list,
                        const struct list_head *head)
{
    return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static int list_empty(const struct list_head *head)
{
    return head->next == head;
}

/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static int list_is_singular(const struct list_head *head)
{
    return !list_empty(head) && (head->next == head->prev);
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

/**
 * list_for_each - iterate over a list
 * @pos:  the struct list_head to use as a loop counter
 * @head: the head of the list
 */
#define list_for_each(pos, head)                                \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_prev -	iterate over a list backwards
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each_prev(pos, head)                           \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head)                        \
    for (pos = (head)->next, n = pos->next; pos != (head);      \
         pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_prev_safe(pos, n, head)   \
    for (pos = (head)->prev, n = pos->prev;     \
         pos != (head);                         \
         pos = n, n = pos->prev)

/**
 * list_for_each_entry - iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @type:       the type of struct that @pos is
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, type, head, member)                    \
    for (pos = list_entry((head)->next, type, member);                  \
         &pos->member != (head);                                        \
         pos = list_entry(pos->member.next, type, member))

#endif /* _LLIST_H_ */
