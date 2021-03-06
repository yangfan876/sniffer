struct parameter *resolve_parameter(int argc, \
			const char **argv);

void print_help(char par);

struct parameter
{
#define PH 0x1
#define PD 0x2
#define PC 0x4
#define PP 0x8
#define PA 0x10

	unsigned char par;
	const char *device_name;
	const char *ip;
	const char *protocol;
	int broadcast;
};

#define PAR_MASK (0x20-1)
