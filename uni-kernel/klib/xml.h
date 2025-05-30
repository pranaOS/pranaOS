/**
 * @file xml.h
 * @author Krisna Pranav
 * @brief xml
 * @version 6.0
 * @date 2025-05-30
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef XML_H_
#define XML_H_
 
typedef struct xml_elem {
    bytes start, len;
    bytes data_start, data_len;
} *xml_elem;

/**
 * @param b 
 * @param name 
 * @param elem 
 * @return boolean 
 */
boolean xml_get_elem(buffer b, sstring name, xml_elem elem);

/**
 * @param b 
 * @param elem 
 * @param name 
 * @param start 
 * @param len 
 * @return boolean 
 */
boolean xml_elem_get_attr(buffer b, xml_elem elem, sstring name, bytes *start, bytes *len);
 
#endif