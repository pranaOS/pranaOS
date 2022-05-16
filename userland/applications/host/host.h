#include <stdint.h>

/**
 * @brief DNS FLAGS 
 * 
 */
#define DNS_FLAG_QUERY 0
#define DNS_FLAG_RESPONSE 1
#define DNS_STANDARD_QUERY 0
#define DNS_A_RECORD 1
#define DNS_CNAME 5

/**
 * @brief dns pakcet
 * 
 */
struct dns_packet {
	uint16_t id;
	uint8_t rd : 1;
	uint8_t tc : 1;
	uint8_t aa : 1;
	uint8_t op_code : 4;
	uint8_t qr : 1;
	uint8_t rcode : 4;
	uint8_t z : 3;
	uint8_t ra : 1;
	uint16_t qd_count;
	uint16_t an_count;
	uint16_t ns_count;
	uint16_t ar_count;
	uint8_t payload[];
};