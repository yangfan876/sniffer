#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "netlib.h"

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

void print_eth(ETHERNET_HEAD *eth)
{
	int i;

	printf("Destnation mac address:");
	
	for (i = 0; i < 5; i++)
	{
		printf("%02x:", eth->dst_mac[i]);
	}
	printf("%02x\n",eth->dst_mac[i]);
	
	printf("Source mac address:");

	for (i = 0; i < 5; i++)
	{
		printf("%02x:", eth->src_mac[i]);
	}
	printf("%02x\n",eth->src_mac[i]);

	switch (ntohs(eth->type))
	{
		case ARP: printf("Upper layer protocol is %s (0x%04x)\n", "ARP", ntohs(eth->type));
				  break;
		case IP: printf("Upper layer protocol is %s (0x%04x)\n", "IP", ntohs(eth->type));
	}
}

void print_arp(ARP_HEAD *arp)
{
	printf("in the ARP package\n");	
}

void print_ip(IP_HEAD *ip)
{
	printf("in the IP package\n");
}
