#include <netinet/in.h>
#include "type.h"

#define ARP 0x0806
#define IP  0x0800

typedef struct eth_head ETHERNET_HEAD;
typedef struct arp_head ARP_HEAD;
typedef struct arp_package ARP_PACKAGE;
typedef struct ip_head IP_HEAD;

struct eth_head
{
	_byte_ dst_mac[6];
	_byte_ src_mac[6];
	_dbyte_ type;
}__attribute__((packed));


struct arp_head
{
	_dbyte_ H_type;
	_dbyte_ P_type;
	_byte_ H_size;
	_byte_ P_size;
	_dbyte_ opcode;
	_byte_ sender_mac[6];
	_word_ sender_ip;
	_byte_ target_mac[6];
	_word_ target_ip;
}__attribute__((packed));


struct arp_package
{
	ETHERNET_HEAD eth;
	ARP_HEAD arp;
}__attribute__((packed));


struct ip_head
{
	_byte_ ip_v:4;
	_byte_ ip_hl:4;
	_byte_ ip_tos;
	_dbyte_ ip_len;
	_dbyte_ ip_id;
	_dbyte_	ip_flag_offset;
#define IP_RF 0x8000
#define IP_DF 0x4000
#define IP_MF 0x2000
#define IP_MASK 0x1fff
	_byte_ ip_ttl;
	_byte_ ip_p;
	_dbyte_ ip_sum;
	struct in_addr ip_src, ip_dst;
}__attribute__((packed));


void fill_the_eth(ETHERNET_HEAD *eth, unsigned char *dst_mac,\
			unsigned char *src_mac, int type);
void fill_the_arp(ARP_HEAD *arp, unsigned char *sender_mac,\
		unsigned char *sender_ip, unsigned char *target_mac,\
		unsigned char *target_ip);
int get_my_mac_addr(char *my_mac, \
			unsigned char *device_name);
int get_gateway_mac (unsigned char *gateway_mac);
void print_eth(ETHERNET_HEAD *eth);
void print_arp(ARP_HEAD *arp);
void print_ip(IP_HEAD *ip);
