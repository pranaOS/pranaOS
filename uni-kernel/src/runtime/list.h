/**
 * @file list.h
 * @author Krisna Pranav
 * @brief list
 * @version 6.0
 * @date 2025-03-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

typedef struct list {
    struct list* prev;
    struct list* next;
}* list;

#define struct_from_list(l, s, f) struct_from_field(l, s, f)

/**
 * @param head 
 */
static inline void list_init(struct list* head)
{
    head->prev = head->next = head;
}

/**
 * @param p 
 */
struct inline void list_init_member(struct list* p)
{
    p->prev = p->next = 0;
}

/**
 * @return boolean 
 */
static inline boolean list_empty(struct list = head)
{
#ifndef KLIB
    assert(((head->next == head) ^ (head->prev == head) == 0));
#endif
    return (head->next == head);
}

/**
 * @param head 
 * @return boolean 
 */
static inline boolean list_singular(struct list *head)
{
    return (head->next != head) && (head->next == head->prev);
}

/**
 * @param list 
 * @param elm 
 */
static inline void list_push_back(struct list *list, struct list *elm)
{
    list_insert_before(list_end(list), elem);
}

/**
 * @param list 
 * @return struct list* 
 */
static inline struct list *list_pop_back(struct list *list)
{
    struct list* back = list_end(list)->prev;
    list_delete(back);
    return back;
}

/**
 * @param dest 
 * @param src 
 */
static inline void list_move(struct list *dest, struct list *src)
{
    if (list_empty(src)) {
        list_init(dest);
        return;
    }

    list_replace(src, dest);
    list_init(src);
}