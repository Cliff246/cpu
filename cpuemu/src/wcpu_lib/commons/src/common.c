
#include "common.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void print_bin(long bin, char len, bool newline)
{
#define BUFSIZE 65
    char buffer[BUFSIZE] = {0};
    for (int i = 0, b = len - 1; i < len; i++, b--)
    {
        char bit = GETBIT(bin, b);
        buffer[i] = (char)(bit + '0');
    }
    printf("%s", buffer);
    if(newline)
    {
        printf("\n");
    }
}

void print_hex(char hex, bool newline)
{
    printf("0x%02hhx%c", hex, (newline)? '\n':0x00);
}

void print_str_hex(char *str, bool newline)
{

    for(char *temp = str; *temp != 0; temp++)
    {
        print_hex(*temp, false);
        if(*temp == '\n')
            printf("->'%s' ", "\\n");
        else
            printf("->'%c' ", *temp);
    }
    if(newline)
        printf("\n");

}


//Maybe return new length
void reverse(char *ary)
{
    size_t len = strlen(ary);
    int end = len;
	if(len == 2)
	{
		char temp = ary[0];
        ary[0] = ary[1];

	    ary[1] = temp;
		return;
	}
    for (int i = 0; i < len / 2; i++, end--)
    {
        char temp = ary[i];
        ary[i] = ary[end];

	    ary[end] = temp;
		printf("%d %d\n", i, end);

	}

}



char *to_string(char ch)
{
	char *str = calloc(2, sizeof(char));
	if(!str)
		return NULL;
	str[0] = ch;
	str[1] = 0;
	return str;
}


void *realloc_safe(void *ptr, size_t count, size_t size)
{

	void *temp = realloc(ptr, size * count);
	if(!temp )
	{
		fprintf(stderr, "realloc failed\n");
		return ptr;
	}
	else
	{
		return temp;
	}
}

