#ifndef __COMMONS__
#define __COMMONS__

#include <stdbool.h>
#include <stdlib.h>

#define SETBIT(n, k) (1 | (n << k))
#define CLRBIT(n, k) (n & ~(1 << k))
#define GETBIT(n, k) (1 & (n >> k))




void print_bin(long bin, char len, bool newline);

void print_hex(char hex, bool newline);
void print_str_hex(char *str, bool newline);
void reverse(char *ary);
#endif
