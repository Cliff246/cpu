#ifndef __COMMONS__
#define __COMMONS__

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum number_type
{
	NUM_NONE,
	NUM_INT,
	NUM_HEX,
	NUM_OCT
}number_type_t;



#define SETBIT(n, k) (n | (1 << (k - 1)))
#define CLRBIT(n, k) (n & (~(1 << (k - 1))))
#define GETBIT(n, k) ((n >> k) & 1)
#define ARYSIZE(ary) sizeof(ary)/sizeof(*ary)

#define MIN(x,y) ((x < y)? x : y)

void *safe_calloc(size_t size, size_t type_size);

void *safe_realloc(void *ptr, size_t size, int type_size);

#define SIZEOF(x) sizeof(x)

#define REALLOC(ptr, size, type) safe_realloc(ptr, size, SIZEOF(type))
#define CALLOC(size, type) safe_calloc(size, SIZEOF(type))

void print_bin(long bin, char len, bool newline);

void print_hex(char hex, bool newline);
void print_str_hex(char *str, bool newline);
void reverse(char *ary);

number_type_t get_number_type(char *str);

int64_t convert_to_hex(char *number);
int64_t convert_to_oct(char *number);
bool valid_name(char *name);
int remove_quotes(char *string, char *buffer, size_t length);


void logit(char *fmt, size_t line, char *filename, ...);

#define DEFAULT_BUFFER 1024
#define LOG(fmt, ...) logit(fmt, __LINE__, __FILE_NAME__, __VA_ARGS__)

#endif
