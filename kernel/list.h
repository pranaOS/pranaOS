#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct node {
	struct node * next;
	struct node * prev;
	void * value;
	struct ListHeader * owner;
} __attribute__((packed)) node_t;

typedef struct ListHeader {
	node_t * head;
	node_t * tail;
	size_t length;
	const char * name;
	const void * metadata;
} __attribute__((packed)) list_t;

/**
 * @brief list destroy
 * 
 * @param list 
 */
extern void list_destroy(list_t * list);

/**
 * @brief list free
 * 
 * @param list 
 */
extern void list_free(list_t * list);

/**
 * @brief list append
 * 
 * @param list 
 * @param item 
 */
extern void list_append(list_t * list, node_t * item);

/**
 * @brief list insert
 * 
 * @param list 
 * @param item 
 * @return node_t* 
 */
extern node_t * list_insert(list_t * list, void * item);

/**
 * @brief list create
 * 
 * @param name 
 * @param metadata 
 * @return list_t* 
 */
extern list_t * list_create(const char * name, const void * metadata);

/**
 * @brief list find
 * 
 * @param list 
 * @param value 
 * @return node_t* 
 */
extern node_t * list_find(list_t * list, void * value);

/**
 * @brief list index of 
 * 
 * @param list 
 * @param value 
 * @return int 
 */
extern int list_index_of(list_t * list, void * value);

/**
 * @brief list remove
 * 
 * @param list 
 * @param index 
 */
extern void list_remove(list_t * list, size_t index);

/**
 * @brief list delete
 * 
 * @param list 
 * @param node 
 */
extern void list_delete(list_t * list, node_t * node);

/**
 * @brief list pop 
 * 
 * @param list 
 * @return node_t* 
 */
extern node_t * list_pop(list_t * list);

/**
 * @brief list dequeue
 * 
 * @param list 
 * @return node_t* 
 */
extern node_t * list_dequeue(list_t * list);

/**
 * @brief list copy
 * 
 * @param original 
 * @return list_t* 
 */
extern list_t * list_copy(list_t * original);

/**
 * @brief list merge
 * 
 * @param target 
 * @param source 
 */
extern void list_merge(list_t * target, list_t * source);

/**
 * @brief list index
 * 
 * @param list 
 * @param index 
 * @return void* 
 */
extern void * list_index(list_t * list, int index);

/**
 * @brief list append after
 * 
 * @param list 
 * @param before 
 * @param node 
 */
extern void list_append_after(list_t * list, node_t * before, node_t * node);

/**
 * @brief list insert after
 * 
 * @param list 
 * @param before 
 * @param item 
 * @return node_t* 
 */
extern node_t * list_insert_after(list_t * list, node_t * before, void * item);

/**
 * @brief list append before
 * 
 * @param list 
 * @param after 
 * @param node 
 */
extern void list_append_before(list_t * list, node_t * after, node_t * node);

/**
 * @brief list insert before
 * 
 * @param list 
 * @param after 
 * @param item 
 * @return node_t* 
 */
extern node_t * list_insert_before(list_t * list, node_t * after, void * item);

/**
 * @brief list foreach
 * 
 */
#ifndef LIST_NO_FOREACH
#  define foreach(i, list) for (node_t * i = (list)->head; i != NULL; i = i->next)
#  define foreachr(i, list) for (node_t * i = (list)->tail; i != NULL; i = i->prev)
#endif
