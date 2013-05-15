#include <stdio.h>

#include "resolve_parameter.h"
#include "thread_cheat.h"

void print_PAR(int argc, const struct parameter *par);

void main(int argc, char **argv)
{
	struct parameter *par;
	const char **PAR = (const char **)argv;

	par = resolve_parameter(argc, PAR);

//	print_PAR(argc, par);

	if (par->par & PC)
	{
		thread_cheat(par);
	}

	while (1)
	{
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
