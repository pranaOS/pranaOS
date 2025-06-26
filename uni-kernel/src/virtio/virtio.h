/**
 * @file virtio.h
 * @author Krisna Pranav
 * @brief virtio
 * @version 6.0
 * @date 2025-06-26
 * 
 * @copyright Copyright (c) 2025
 * 
*/

/**
 * @param kh 
 */
void init_virtio_9p(kernel_heaps kh);

/**
 * @param kh 
 */
void init_virtio_balloon(kernel_heaps kh);

/**
 * @param kh 
 * @param a 
 */
void init_virtio_blk(kernel_heaps kh, storage_attach a);

/**
 * @param kh 
 */
void init_virtio_network(kernel_heaps kh);

/**
 * @param kh 
 */
void init_virtio_rng(kernel_heaps kh);

/**
 * @param kh 
 * @param a 
 */
void init_virtio_scsi(kernel_heaps kh, storage_attach a);

/**
 * @param kh 
 */
void init_virtio_socket(kernel_heaps kh);

/**
 * @param kh 
 */
void virtio_mmio_enum_devs(kernel_heaps kh);
