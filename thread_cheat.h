#include <netinet/in.h>

void cheat_target (void *argv);
void thread_cheat(const struct parameter *par);

struct cheat_argv
{
	int broad;
	char *device_name;
	char *sender_ip;
	char *target_ip; 
};
