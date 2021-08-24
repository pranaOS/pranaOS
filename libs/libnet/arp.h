#ifndef ARP_H
#define ARP_H

#define ETHTYPE_ARP 0x0806
#define ETHTYPE_IP  0x0800

void arp_init();
void arp_ip_input(struct netif *netif, struct pbuf *p);
struct pbuf *arp_arp_input(struct netif *netif, struct eth_addr *ethaddr, struct pbuf *p);
struct eth_addr *arp_lookup(struct ip_addr *ipaddr);
struct pbuf *arp_query(struct netif *netif, struct eth_addr *ethaddr, struct ip_addr *ipaddr);
int arp_queue(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr);

#endif