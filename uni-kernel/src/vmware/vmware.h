/**
 * @file vmware.h
 * @author Krisna Pranav
 * @brief vmware
 * @version 6.0
 * @date 2025-07-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

/**
 * @param kh 
 * @param a 
 */
void init_pvscsi(kernel_heaps kh, storage_attach a);

/**
 * @param kh 
 */
void init_vmxnet3_network(kernel_heaps kh);