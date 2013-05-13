#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "resolve_parameter.h"

void cheat_target (void *argv)
{
	const struct parameter *par = (struct parameter *)argv;
	int cheat_socket;

	printf("in the thread,print the parameter.\n");

	
	
}

void thread_cheat(const struct parameter *par)
{
	pthread_t tidp;
	
	pthread_create(&tidp, NULL, (void *(*)(void *))cheat_target, (void *)par);	
	
	return;
}
