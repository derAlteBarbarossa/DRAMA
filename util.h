#ifndef UTIL_H
#define UTIL_H

#define SUPERPAGE 1024*1024
#define POOLSIZE 1024
#define ROUNDS 1000
#define GROUPINGTHRESHOLD	20
#define THRESHOLDROUNDS		10

struct address_in_bank
{
	char* address;
	struct address_in_bank* next_address;
};

extern char* allocate_superpage();
extern int compare( const void* a, const void* b);
extern int find_median(int times[]);
extern int time_access(char const *addr1, char const *addr2);
extern char** initialise_pool(char* buffer);
extern int un_coloured(int colours[]);
extern void revert_colour(int colours[], int current_colour);
extern void bank_conflict(char* buffer);
extern void find_banks(char* buffer, int threshold);
extern int find_threshold(char* buffer);


#endif // UTIL_H
