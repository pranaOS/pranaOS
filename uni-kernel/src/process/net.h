/**
 * @file net.h
 * @author Krisna Pranav
 * @brief net
 * @version 6.0
 * @date 2025-08-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
*/

#define NET 1
#define NET_SYSCALLS 1

/**
 * @param kh 
 */
void init_net(kernel_heaps kh);

/**
 * @param root 
 * @param m 
 */
void init_network_iface(tuple root, merge m);

/**
 * @param h 
 * @param port 
 * @param c 
 * @return status 
 */
status listen_port(heap h, u16 port, connection_handler c);