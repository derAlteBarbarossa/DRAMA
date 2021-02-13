#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char* argv[])
{
	int options;
	int bflag = 0;
	int tflag = 0;
	char *tvalue = NULL;

	options = getopt (argc, argv, "bt:");
	
	if(options == -1)
	{
		char* buffer = allocate_superpage();
		bank_conflict(buffer);
	}
	else if(options == 'b')
	{
		char* buffer = allocate_superpage();
		find_banks(buffer, find_threshold(buffer));
	}
	else if(options == 't')
	{
		tvalue = optarg;
		char* buffer = allocate_superpage();
		find_banks(buffer, atoi(tvalue));
	}
	
    return 0;
}
