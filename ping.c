#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define True 1
#define False 0

void usage(void);
u_short in_cksum(const u_short *addr, register int len, u_short csum);
u_short my_cksum(const u_short *addr, int lenth);
u_short my__cksum(const u_short *addr, int lenth);


struct argument
{
	int lenth;
	int broadcast;
	char *target_ip;
};

void main(int argc, char **argv)
{
	int icmp_sock;
	int opt, parcount = 0;
	struct argument par = {0};
	struct icmphdr icmp_request = {0};
	struct sockaddr_in to = {0};
	unsigned short a = 0;

/*
 *
 *
 * */

	while ((opt = getopt(argc, argv, "l:b?")) != -1)
	{
		switch (opt)
		{
			case 'l': par.lenth = atoi(optarg);
					  parcount += 2;
					  break;
			case 'b': par.broadcast = True;
					  parcount += 1;
					  break;
			case '?': usage();
					  exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	if (argc <= 0)
	{
		usage();
		exit(1);
	}
	
	par.target_ip = *argv;


	to.sin_family = AF_INET;
	to.sin_addr.s_addr = inet_addr(par.target_ip);

/*
 *
 *
 *
 * */

	if (par.broadcast)
	{
		setsockopt(icmp_sock, IPPROTO_IP, SO_BROADCAST, \
				NULL, 0);
	}

	icmp_request.type = ICMP_ECHO;
	icmp_request.code = 0;
	icmp_request.un.echo.sequence = htons(1);
	icmp_request.un.echo.id = htons(getpid());

	if ((icmp_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		perror("SOCKET");
		exit(1);
	}

	while (1)
	{

		icmp_request.un.echo.sequence += htons(1);
		icmp_request.checksum = 0;
//		icmp_request.checksum = in_cksum((u_short *)&icmp_request, (par.lenth ? par.lenth : 64), 0);
		icmp_request.checksum = my_cksum((u_short *)&icmp_request, (par.lenth ? par.lenth : 64));
		sendto(icmp_sock, (void *)&icmp_request, (par.lenth ? par.lenth : 64), \
					0, (struct sockaddr *)&to, sizeof(to));
		sleep(1);
	
	}

	return;
}

void usage(void)
{
	printf("helpful infomation\n");
}

typedef unsigned short u_short;

u_short in_cksum(const u_short *addr, register int len, u_short csum)
{
	register int nleft = len;
	const u_short *w = addr;
	register u_short answer;
	register int sum = csum;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1)
		sum += *(u_char *)w; /* le16toh() may be unavailable on old systems */

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

u_short my_cksum(const u_short *addr, int lenth)
{
	const u_short *w = addr;
	register u_short end;
	register int sum = 0;
	register int len = lenth;
	
	while (len > 1)
	{
		sum += *w++;
		len -= 2;
	}

	if (len)
	{
		sum += *(unsigned char *)w;
	}

	while (sum >> 16)
	{
		sum = (sum >> 16) + (sum & 0xffff);
//		sum += (sum >> 16);
	}

	end = ~sum;
	return (end);
}


