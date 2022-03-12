/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _LIST_H_
#define _LIST_H_

#include <macro.h>

struct list_head {
    struct list_head *prev, *next;
};

#define LIST_HEAD(name) \
    struct list_head name = { &(name), &(name) }

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

typedef long (*list_cmp_t)(struct list_head *, struct list_head *, void *);
extern void list_sort(struct list_head *head, list_cmp_t cmp, void *data);

static inline void list_insert(struct list_head *prev, struct list_head *next,
                               struct list_head *new)
{
    prev->next = new;
    next->prev = new;
    new->prev = prev;
    new->next = next;
}

static inline void list_head_init(struct list_head *head)
{
    head->prev = head;
    head->next = head;
}

/**
 * list_add - add a new node next old node.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void list_add(struct list_head *node, struct list_head *new)
{
    list_insert(node, node->next, new);
}

/**
 * list_add_prev - add a new node after old node.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void list_add_prev(struct list_head *node, struct list_head *new)
{
    list_insert(node->prev, node, new);
}

/**
 * list_del - in fact, it just connect next and prev node.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void list_del(struct list_head *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/**
 * list_replace - Replace a linked list node with an external node.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->prev = old->prev;
    new->next = old->next;
    new->prev->next = new;
    new->next->prev = new;
}

/**
 * list_move_front - Move the node to the tail of the list.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void list_move_front(struct list_head *head, struct list_head *node)
{
    list_del(node);
    list_add(head, node);
}

/**
 * list_move_tail - Move the node to the tail of the list.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void list_move_tail(struct list_head *head, struct list_head *node)
{
    list_del(node);
    list_add_prev(head, node);
}

/**
 * list_check_first - Check whether the node is a header.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline bool list_check_first(struct list_head *head, struct list_head *node)
{
    return node->prev == head;
}

/**
 * list_check_end - Check whether the node is a ending.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline bool list_check_end(struct list_head *head, struct list_head *node)
{
    return node->next == head;
}

/**
 * list_check_around - Check whether the node is a ending.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline bool list_check_around(struct list_head *prev, struct list_head *next)
{
    return prev->next == next;
}

/**
 * list_check_empty - add a new node next old node.
 * @head: list head to check.
 */
static inline bool list_check_empty(struct list_head *head)
{
    return head->next == head;
}

/**
 * list_check_head - test if the entry points to the head of the list
 * @pos: the type * to cursor
 * @head: the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_check_head(pos, head, member) \
    (&(pos)->member == (head))

/**
 * list_first_entry - get the first element from a list
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

/**
 * list_prev_entry - get the prev element in list
 * @pos: the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_prev_entry(pos, member) \
    list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * list_next_entry - get the next element in list
 * @pos: the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_entry_is_head - test if the entry points to the head of the list
 * @pos: the type * to cursor
 * @head: the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_entry_is_head(pos, head, member) \
    (&pos->member == (head))

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) ({          \
    struct list_head *head__ = (ptr);                           \
    struct list_head *pos__ = head__->next;                     \
    pos__ != head__ ? list_entry(pos__, type, member) : NULL;   \
})

/**
 * list_for_each - iterate over a list
 * @head: the head for your list.
 * @pos: the &struct list_head to use as a loop cursor.
 */
#define list_for_each(head, pos) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_entry - iterate over list of given type
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member)                  \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                                \
         pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos: the type * to use as a loop cursor.
 * @next: another type * to use as temporary storage
 * @head: the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, next, head, member)       \
    for (pos = list_first_entry(head, typeof(*pos), member),    \
         next = list_next_entry(pos, member);                   \
         !list_entry_is_head(pos, head, member);                \
         pos = next, next = list_next_entry(next, member))

#endif  /* _LIST_H_ */
