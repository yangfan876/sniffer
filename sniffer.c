#include <stdio.h>
#include "resolve_parameter.h"

void print_PAR(int argc, struct parameter *par);

void main(int argc, char **argv)
{
	struct parameter *par;
	const char *PAR = argv;

	par = resolve_parameter(argc, PAR);

	print_PAR(argc, par);
}


void print_PAR(int argc, struct parameter *par)
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
