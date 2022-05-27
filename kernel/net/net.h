#ifndef KERNEL_NET_H
#define KERNEL_NET_H

#include <ak/ether.h>
#include <ak/list.h>
#include <filesystem/vfs.h>

#define AF_UNIX 1	 
#define AF_INET 2	 
#define AF_PACKET 17 

#define PF_UNIX AF_UNIX
#define PF_INET AF_INET

#define PF_PACKET AF_PACKET

#define WORD_SIZE 4
#define WORD_MASK (~(WORD_SIZE - 1))
#define WORD_ALIGN(addr) (((addr) + WORD_SIZE - 1) & WORD_MASK)

#define CHECKSUM_MASK 0xFFFF

struct sk_buff;

enum {
	IPPROTO_IP = 0,	   
	IPPROTO_ICMP = 1,  
	IPPROTO_TCP = 6,   
	IPPROTO_UDP = 17,  
	IPPROTO_RAW = 255, 
	IPPROTO_MAX
};

typedef unsigned short sa_family_t;

struct sockaddr {
	sa_family_t sa_family; 
	char sa_data[14];	   
};

struct sockaddr_in {
	uint16_t sin_port; 
	uint32_t sin_addr; 
};

#define PACKET_HOST 0	   
#define PACKET_BROADCAST 1 
#define PACKET_MULTICAST 2 
#define PACKET_OTHERHOST 3 
#define PACKET_OUTGOING 4  
#define PACKET_LOOPBACK 5  
#define PACKET_USER 6	   
#define PACKET_KERNEL 7	   
#define PACKET_FASTROUTE 6 

struct sockaddr_ll {
	uint16_t sll_protocol; 
	uint8_t sll_pkttype;   
	uint8_t sll_addr[6];   
};

#define IFNAMSIZ 16

struct ifmap {
	unsigned long mem_start;
	unsigned long mem_end;
	unsigned short base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
};

struct ifreq {
	char ifr_name[IFNAMSIZ]; 
	union {
		struct sockaddr ifr_addr;
		struct sockaddr ifr_dstaddr;
		struct sockaddr ifr_broadaddr;
		struct sockaddr ifr_netmask;
		struct sockaddr ifr_hwaddr;
		short ifr_flags;
		int ifr_ifindex;
		int ifr_metric;
		int ifr_mtu;
		struct ifmap ifr_map;
		char ifr_slave[IFNAMSIZ];
		char ifr_newname[IFNAMSIZ];
		char *ifr_data;
	};
};

enum socket_state {
	SS_UNCONNECTED = 1,	  
	SS_CONNECTING = 2,	  
	SS_CONNECTED = 3,	  
	SS_DISCONNECTING = 4, 
	SS_DISCONNECTED = 5,
};

enum socket_type {
	SOCK_STREAM = 1,
	SOCK_DGRAM = 2,
	SOCK_RAW = 3,
};

struct socket {
	uint16_t protocol;
	enum socket_type type;
	enum socket_state state;
	uint32_t flags;

	struct vfs_file *file;
	struct sock *sk;
	struct proto_ops *ops;

	struct list_head sibling;
};

struct sock
{
	struct socket *sock;
	struct net_device *dev;
	struct thread *owner_thread;
	struct list_head rx_queue;
	uint32_t rx_length;
	struct list_head tx_queue;
	struct list_head *send_head;
};

struct inet_sock {
	struct sock sk;
	struct sockaddr_in ssin;
	struct sockaddr_in dsin;
};

struct packet_sock {
	struct sock sk;
	struct sockaddr_ll sll;
};

static inline struct packet_sock *pkt_sk(struct sock *sk) {
	return (struct packet_sock *)sk;
}

static inline struct inet_sock *inet_sk(struct sock *sk) {
	return (struct inet_sock *)sk;
}

struct proto_ops {
	int family;
	int obj_size;
	int (*bind)(struct socket *sock, struct sockaddr *myaddr, int sockaddr_len);
	int (*connect)(struct socket *sock, struct sockaddr *vaddr, int sockaddr_len);
	int (*accept)(struct socket *sock, struct sockaddr *addr, int sockaddr_len);
	int (*ioctl)(struct socket *sock, unsigned int cmd, unsigned long arg);
	int (*listen)(struct socket *sock, int backlog);
	int (*shutdown)(struct socket *sock);
	int (*sendmsg)(struct socket *sock, void *msg, size_t msg_len);
	int (*recvmsg)(struct socket *sock, void *msg, size_t msg_len);
	int (*handler)(struct socket *sock, struct sk_buff *skb);
};

struct socket_alloc {
	struct socket socket;
	struct vfs_inode inode;
};

static inline struct socket *SOCKET_I(struct vfs_inode *inode) {
	return &container_of(inode, struct socket_alloc, inode)->socket;
}

static inline struct vfs_inode *SOCK_INODE(struct socket *socket) {
	return &container_of(socket, struct socket_alloc, socket)->inode;
}

enum netdev_state {
	NETDEV_STATE_OFF = 1,
	NETDEV_STATE_UP = 1 << 1,
	NETDEV_STATE_CONNECTED = 1 << 2,  
};

struct net_device {
	uint32_t base_addr;
	uint8_t irq;

	char name[16];
	enum netdev_state state;
	struct list_head sibling;
	uint8_t dev_addr[6];
	uint8_t broadcast_addr[6];
	uint8_t zero_addr[6];
	uint8_t router_addr[6];
	uint16_t mtu;
	uint32_t dns_server_ip;
	uint32_t dhcp_server_ip;
	uint32_t router_ip;
	uint32_t local_ip;
	uint32_t subnet_mask;
	uint32_t lease_time;
};

void net_init();
void net_rx_loop();
void net_switch();
void push_rx_queue(uint8_t *data, uint32_t size);
void socket_setup(int32_t family, enum socket_type type, int32_t protocal, struct vfs_file *file);
int socket_shutdown(struct socket *sock);
struct socket *sockfd_lookup(uint32_t fd);
uint16_t singular_checksum(void *packet, uint16_t size);
uint32_t packet_checksum_start(void *packet, uint16_t size);
uint16_t transport_calculate_checksum(void *segment, uint16_t segment_len, uint8_t protocal, uint32_t source_ip, uint32_t dest_ip);
char *inet_ntop(uint32_t src, char *dst, uint16_t len);
int inet_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);

void register_net_device(struct net_device *);
struct net_device *get_current_net_device();

extern struct proto_ops tcp_proto_ops;
extern struct proto_ops udp_proto_ops;
extern struct proto_ops raw_proto_ops;
extern struct proto_ops packet_proto_ops;

#endif
