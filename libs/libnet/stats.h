#ifndef STATS_H
#define STATS_H

struct stats_nic {
  unsigned long rx_packets;             
  unsigned long tx_packets;             
  unsigned long rx_bytes;               
  unsigned long tx_bytes;               
  unsigned long rx_errors;              
  unsigned long tx_errors;              
  unsigned long rx_dropped;             
  unsigned long tx_dropped;             
  unsigned long multicast;              
  unsigned long collisions;

  unsigned long rx_length_errors;
  unsigned long rx_over_errors;         
  unsigned long rx_crc_errors;          
  unsigned long rx_frame_errors;        
  unsigned long rx_fifo_errors;         
  unsigned long rx_missed_errors;       

  unsigned long tx_aborted_errors;
  unsigned long tx_carrier_errors;
  unsigned long tx_fifo_errors;
  unsigned long tx_heartbeat_errors;
  unsigned long tx_window_errors;

  unsigned long rx_compressed;
  unsigned long tx_compressed;
};

struct stats_proto {
  unsigned long xmit;   
  unsigned long rexmit; 
  unsigned long recv;    
  unsigned long fw;      
  unsigned long drop;    
  unsigned long chkerr;  
  unsigned long lenerr;  
  unsigned long memerr;  
  unsigned long rterr;   
  unsigned long proterr; 
  unsigned long opterr;  
  unsigned long err;     
};

struct stats_pbuf {
  unsigned long avail;
  unsigned long used;
  unsigned long max;  
  unsigned long err;
  unsigned long reclaimed;

  unsigned long alloc_locked;
  unsigned long refresh_locked;

  unsigned long rwbufs;
};

struct netstats {
  struct stats_proto link;
  struct stats_proto ip;
  struct stats_proto icmp;
  struct stats_proto udp;
  struct stats_proto tcp;
  struct stats_proto raw;
  struct stats_pbuf pbuf;
};

extern struct netstats stats;

krnlapi extern struct netstats *get_netstats();

void stats_init();

#endif