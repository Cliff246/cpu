#ifndef __COMMON__
#define __COMMON__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SETBIT(n, k) (1 | (n << k))
#define CLRBIT(n, k) (n & ~(1 << k))
#define GETBIT(n, k) (1 & (n >> k))



void print_bin(long bin, char len, bool newline);

void print_hex(char hex, bool newline);
void print_str_hex(char *str, bool newline);
void reverse(char *ary);
//returns new calloc'd string of length 2 with char 0 being ch
char *to_string(char ch);

void *realloc_safe(void *ptr, size_t count, size_t size); 
#endif

