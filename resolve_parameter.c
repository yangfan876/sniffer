#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resolve_parameter.h"

struct parameter *resolve_parameter(int argc, \
			const char **argv)
{
	int i;
	struct parameter *par;

	par = (struct parameter *) \
		  malloc(sizeof(struct parameter));

	memset(par, 0, sizeof(struct parameter));

	for (i = 1; i < argc; i++)
	{
		if((argv[i][0] != '-' && argv[i-1][0] != '-') || \
				(argv[i][0] == '-' && argv[i-1][0] == '-'))
		{
			print_help(argv[i][1]);
			exit(1);
		}
		if(argv[i][0] == '-' && argv[i][1] != '-')
		{
			switch (argv[i][1])
			{
				case 'h':
					par->par |= PH;
					print_help('N');
					exit(1);
					break;
	
				case 'd':
					par->par |= PD;
					par->device_name = argv[i+1];
					break;
	
				case 'c': 
					par->par |= PC;
					par->ip = argv[i+1];
					break;
				
				case 'p':
					par->par |= PP;
					par->protocol = argv[i+1];
					break;

				case 'A':
					par->par |= PA;
					par->broadcast = 1;
					break;
	
				default:
						  print_help(argv[i][1]);
						  exit(1);
						  break;
			}
		}
	}
	if (!(par->par & PD))
	{
		printf("You must spcify a device!\n");
		exit(1);
	}

	return (par);
}

void print_help(char par)
{
	if(par != 'N')
	{
		printf("sniffer: invalid option --%c\n", par);
	}

	printf("Usage: sniffer [OPTION]...\n");
	printf("Listen packages form the netcard.\nTell a lie to the gateway.\nTake the target IP host's packages.\n\n");
	printf("    -h\tList the help infomation.\n");
	printf("    -d\tSpecify the net device.\n");
	printf("    -c\tSpecify the IP you want to cheat.\n");
	printf("    -p\tSpecify the protocol of the package you want to get.\n\n");
	return;
}
