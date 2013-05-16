#include <stdio.h>
#include <string.h>
#include <net/ethernet.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "resolve_parameter.h"
#include "thread_cheat.h"
#include "netlib.h"

void print_PAR(int argc, const struct parameter *par);

void main(int argc, char **argv)
{
	struct parameter *par;
	const char **PAR = (const char **)argv;
	struct ifreq setpro = {0};
	int lenth;
	int snif_sock;
	unsigned char buf[2048];
	ETHERNET_HEAD *eth;
	ARP_HEAD *arp;
	IP_HEAD *ip;
	

	par = resolve_parameter(argc, PAR);

	if (par->par & PC)
	{
		thread_cheat(par);
	}
	
	if ((snif_sock = socket(AF_PACKET, SOCK_RAW, \
					htons(ETH_P_ALL))) == -1)
	{
		perror("Create sniffer socket");
		exit(1);
	}

	strncpy(setpro.ifr_name, par->device_name, IFNAMSIZ);
	if (ioctl(snif_sock, SIOCGIFFLAGS, &setpro) == -1)
	{
		perror("ioctl get");
		exit(1);
	}

	setpro.ifr_flags |= IFF_PROMISC;

	if(ioctl(snif_sock, SIOCSIFFLAGS, &setpro) == -1)
	{
		perror("ioctl set");
		exit(1);
	}

	while (1)
	{
	
		lenth = recvfrom(snif_sock, buf, 2048, 0, NULL, NULL);

		if (lenth < 42)
		{
			continue;
		}

		eth = (ETHERNET_HEAD *)&buf[0];
		print_eth (eth);
		switch (ntohs(eth->type))
		{
			case ARP: arp = (ARP_HEAD *)&buf[sizeof(eth)];
					  print_arp(arp);
					  break;
			case IP: ip = (IP_HEAD *)&buf[sizeof(eth)];
					 print_ip(ip);
					 break;
		}
	}
}


void print_PAR(int argc, const struct parameter *par)
{
	int i;
	
	for (i = 0; i < argc; i++)
	{
		switch (par->par & 1<<i)
		{
			case PH:
				print_help('N');
				break;
			case PD:
				printf("%s\n", par->device_name);
				break;
			case PC:
				printf("%s\n", par->ip);
				break;
			case PP:
				printf("%s\n", par->protocol);
				break;
		}
	}
}
