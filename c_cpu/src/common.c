
#include "common.h"
#include <string.h>
#include <stdio.h>

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
    char tstr[len + 1];
    memset(tstr, 0, len + 1);
    strcpy(tstr, ary);
    int initial = 0, end = len - 1;
    for (int i = initial; i < end; i++)
    {
        char temp = tstr[i];
        tstr[i] = tstr[end];
        tstr[end] = temp;
        end--;
    }
    strcpy(ary, tstr);

}
