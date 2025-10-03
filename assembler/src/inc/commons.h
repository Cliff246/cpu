#ifndef __COMMONS__
#define __COMMONS__

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


#define SETBIT(n, k) (1 | (n << k))
#define CLRBIT(n, k) (n & ~(1 << k))
#define GETBIT(n, k) (1 & (n >> k))
#define ARYSIZE(ary) sizeof(ary)/sizeof(*ary)


void *safe_calloc(size_t size, size_t type_size);

void *safe_realloc(void *ptr, size_t size, int type_size);

#define SIZEOF(x) sizeof(x)

#define REALLOC(ptr, size, type) safe_realloc(ptr, size, SIZEOF(type))
#define CALLOC(size, type) safe_calloc(size, SIZEOF(type))

void print_bin(long bin, char len, bool newline);

void print_hex(char hex, bool newline);
void print_str_hex(char *str, bool newline);
void reverse(char *ary);

int get_number_type(char *str);

int64_t convert_to_hex(char *number);
int64_t convert_to_oct(char *number);
bool valid_name(char *name);



#endif
