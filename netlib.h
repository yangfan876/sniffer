#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "type.h"


#define ARP 0x0806


typedef struct eth_head ETHERNET_HEAD;
typedef struct arp_head ARP_HEAD;
typedef struct arp_package ARP_PACKAGE;

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


typedef struct
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
}IP_HEAD;


void fill_the_eth(ETHERNET_HEAD *eth, unsigned char *dst_mac, \
			unsigned char *src_mac, int type)
{
	int i;

	for(i = 0; i < 6; i++)
	{
		eth->dst_mac[i] = dst_mac[i];
		eth->src_mac[i] = src_mac[i];
	}
	eth->type = htons(type);

	return;
}

void fill_the_arp(ARP_HEAD *arp, unsigned char *sender_mac,\
		unsigned char *sender_ip, unsigned char *target_mac,\
		unsigned char *target_ip)
{
	int i;

	arp->H_type = htons(0x0001);
	arp->P_type = htons(0x0800);
	arp->H_size = 0x6;
	arp->P_size = 0x4;
	arp->opcode = htons(0x2);
	arp->sender_ip = inet_addr(sender_ip);
	arp->target_ip = inet_addr(target_ip);
	
	for (i = 0; i < 6; i++)
	{
		arp->sender_mac[i] = sender_mac[i];
		arp->target_mac[i] = target_mac[i];
	}

	return;
}

int get_my_mac_addr(char *my_mac, \
			unsigned char *device_name)
{
	struct ifreq ifreq = {0};
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("get_my_mac_addr --> socket");
		exit(1);
	}

	strcpy(ifreq.ifr_name, device_name);

	if((ioctl(sock, SIOCGIFHWADDR, &ifreq)) == -1)
	{
		perror("ioctl");
		return (0);
	}

	strncpy(my_mac, ifreq.ifr_hwaddr.sa_data, 6);

	my_mac[6] = '\0';

	return 1;
}

int get_gateway_mac (unsigned char *gateway_mac)
{
	int fd, i = 0, k = 0;
	char buf[138], *ptr;
	
	while (1)
	{
		
		fd = open("/proc/net/arp", O_RDONLY);
		
		if(read(fd, buf, 137) < 137)
		{
			printf("read: cannot get the gateway mac address.try later.\n");
			sleep(2);

			close(fd);

			continue;
		}
		else
		{
			break;
		}
		
		i++;

		if (i > 10)
		{
			break;
		}
	}
	
	ptr = &buf[120];

	for (i = 0; i < 6; i++)
	{
		gateway_mac[i] = ((ptr[k] >= 48 && ptr[k] <= 57) ? ((ptr[k]-48)<<4) : ((ptr[k]-87)<<4)) + \
	 					 ((ptr[k+1] >= 48 && ptr[k+1] <= 57) ? (ptr[k+1]-48) : (ptr[k+1]-87));

		k += 3;
	}

	return 1;
}

