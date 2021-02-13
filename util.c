#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <x86intrin.h>

#include "util.h"

char* allocate_superpage()
{
	char* buffer = mmap((void*) 0x40000000, SUPERPAGE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS|MAP_HUGETLB, -1, 0);
	for (int i=0; i<SUPERPAGE; i++)
	    *(buffer+i) = 't';

	return buffer;
}

int compare( const void* a, const void* b)
{
	int int_a = * ( (int*) a );
	int int_b = * ( (int*) b );

	if ( int_a == int_b )
		return 0;
	else if ( int_a < int_b )
		return -1;
	else
		return 1;
}

int find_median(int times[ROUNDS])
{
	qsort(times, ROUNDS, sizeof(int), compare);
	return times[ROUNDS/2];
}

int time_access(char const *addr1, char const *addr2)
{
	int times[ROUNDS];
	int aux = 0;

	volatile char* a1 = (volatile char*) addr1;
	volatile char* a2 = (volatile char*) addr2;

	for (int i=0; i<ROUNDS; i++)
	{
		_mm_mfence();
		uint64_t start = __rdtscp(&aux);
		*a1;
		*a2;
		_mm_mfence();
		times[i] = (__rdtscp(&aux) - start);

		_mm_clflush((char*)a1);
		_mm_clflush((char*)a2);
		

	}

	int median = find_median(times);
	return median;
}



char** initialise_pool(char* buffer)
{
	char** pool = (char**) malloc(sizeof(char*)*POOLSIZE);
	for (int i=0; i<POOLSIZE; i++)
		*(pool + i) = buffer + (rand()%SUPERPAGE);
	return pool;
}

int un_coloured(int colours[])
{
	int not_coloured = 0;

	for (int i=0; i<POOLSIZE; i++)
		if(colours[i] ==0)
			not_coloured++;

	return not_coloured;
}

void revert_colour(int colours[], int current_colour)
{
	for (int i = 0; i < POOLSIZE; i++)
	{
		if(colours[i] == current_colour)
			colours[i] = 0;
	}
}

void bank_conflict(char* buffer)
{
	FILE* fptr;
	fptr = fopen("timing","w");

	char** pool = initialise_pool(buffer);

	int access_time;
	char* base = buffer;

	for (int i=0; i<POOLSIZE; i++)
	{
			access_time = time_access(base, *(pool+i));
			fprintf(fptr, "%p,%p,%d\n", base, *(pool+i), access_time);
	}
	fclose(fptr);
}

void find_banks(char* buffer, int threshold)
{
	int colours[POOLSIZE] = {0};

	char** pool = initialise_pool(buffer);

	int access_time;
	int group_members;
	int current_colour = 1;

	uint ignored = 0;

	for (int i=0; i<POOLSIZE; i++)
	{
		if (colours[i] == 0)
		{
			group_members = 0;
			colours[i] = current_colour;
			for (int j=i+1; j<POOLSIZE; j++)
			{
				if (colours[j] == 0)
				{
					access_time = time_access(*(pool+i), *(pool+j));
					if(access_time > threshold)
					{
						group_members++;
						colours[j] = current_colour;
					}
				}

				
			}
			if(group_members < GROUPINGTHRESHOLD)
			{
				revert_colour(colours, current_colour);
				current_colour--;
			}
			current_colour++;
		}
	}
	printf("%d\n", --current_colour);
}

int find_threshold(char* buffer)
{
	int times[POOLSIZE] = {0};
	char** pool = initialise_pool(buffer);

	char* base = buffer;

	double threshold = 0;
	for (int round = 0; round < THRESHOLDROUNDS; round++)
	{
		for (int i=0; i<POOLSIZE; i++)
		{
			times[i] = time_access(base, *(pool+i));
		}
		qsort(times, POOLSIZE, sizeof(int), compare);
		
		int threshold_pos = 0;
		int max_gap = 0;
		for (int i = 0; i < POOLSIZE-1; i++)
		{
			if(times[i+1]-times[i] > max_gap)
			{
				max_gap = times[i+1]-times[i];
				threshold_pos = i;
			}
		}
		threshold += (double)times[threshold_pos];
	}

	
	printf("%d\t", (int)threshold/THRESHOLDROUNDS);
	return (int) threshold/THRESHOLDROUNDS;
}
