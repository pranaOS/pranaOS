/**
 * @file search.h
 * @author Krisna Pranav
 * @brief Search
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

struct search_tree_node
{

}; // struct search_tree_node

/**
 * @param key 
 * @return struct search_tree_node* 
 */
struct search_tree_node* new_tree_node(void const* key);

/**
 * @param node 
 */
void delete_node_recursive(struct search_tree_node* node);
