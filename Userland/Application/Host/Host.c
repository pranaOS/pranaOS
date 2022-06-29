#include <arpa/inet.h>
#include <ak/ether.h>
#include <math.h>
#include <sockios.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "host.h"

#define MAX_DNS_LEN 512

/**
 * @brief ioctl device addr 
 * 
 * @param type 
 * @return uint32_t 
 */
uint32_t ioctl_device_addr(int type) {
	struct ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;

	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	ioctl(fd, type, (unsigned long)&ifr);
	close(fd);

	return ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
}

/**
 * @brief dns build header
 * 
 * @param dns 
 * @param identifier 
 */
void dns_build_header(struct dns_packet *dns, uint16_t identifier) {
	dns->id = htons(identifier);
	dns->qr = DNS_FLAG_QUERY;
	dns->op_code = DNS_STANDARD_QUERY;
	dns->aa = 0;
	dns->tc = 0;
	dns->rd = 1;
	dns->ra = 0;
	dns->z = 0;
	dns->rcode = 0;
	dns->qd_count = htons(1);
	dns->an_count = 0;
	dns->ns_count = 0;
	dns->ar_count = 0;
}