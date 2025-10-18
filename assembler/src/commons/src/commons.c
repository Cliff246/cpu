
#include "commons.h"
#include "eerror.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>


void *safe_calloc(size_t size, size_t type_size)
{
	void *ptr = calloc(size, type_size);
	//printf("calloc(%lu,%lu)\n", size, type_size);
	if(!ptr)
	{
		ERR(ENOMEM, "calloc failed");
		exit(1);
	}
	return ptr;

}

void *safe_realloc(void *ptr, size_t size, int type_size)
{
	if(ptr == NULL)
	{
		ERR(EINVAL, "realloc argument was NULL");
		return NULL;
	}
	else
	{
		//printf("realloc: %zu\n", size * type_size);
		void *temp = realloc(ptr, size * type_size);
		if(temp == NULL)
		{
			ERR(ENOMEM, "realloc failed");
			exit(1);
		}
		ptr = temp;
		return ptr;
	}
}


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
    for (int i = 0, j = len -1; i < j; i++, j--)
    {
        char temp = ary[i];
        ary[i] = ary[j];
        ary[j] = temp;
    }

}



number_type_t get_number_type(char *str)
{

	size_t len = strlen(str);
	if(len > 0)
	{

		bool isnumber = true;

		if(len == 1)
		{
			return (isdigit(str[0]))? NUM_INT: NUM_NONE;
		}
		else if(len > 1)
		{
			char c1 = str[0];
			char c2 = str[1];
			//standard integer number
			if(isdigit(c1) && isdigit(c2) && c1 != '0')
			{
				for(int i = 2; i < len; ++i)
				{
					if(!isdigit(str[i]))
						return NUM_NONE;

				}
				return NUM_INT;
			}
			//standard hex number
			else if(c1 == '0' && (c2 == 'x' || c2 == 'X'))
			{
				for(int i = 2; i < len; ++i)
				{
					if((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F') || isdigit(str[i]))
						continue;
					else
						return NUM_NONE;

				}
				return NUM_HEX;
			}
			else if(c1 == '-' && isdigit(c2))
			{
				for(int i = 2; i < len; ++i)
				{
					if(!isdigit(str[i]))
						return NUM_NONE
							;

				}
				return NUM_INT;
			}
			else if(c1 == '0' && isdigit(c2))
			{
				for(int i = 2; i < len; ++i)
				{
					if(str[i] < '0' || str[i] > 8)
						return NUM_NONE;

				}
				return NUM_OCT;
			}


		}

	}
	return NUM_NONE;

}

int64_t convert_to_hex(char *number)
{
	int64_t hexval  = 0;
	char *rev = strdup(number);
	reverse(rev);
	int count = 1;
	int base = 1;
	for(char *c = rev; *c; c++)
	{
		int val;
		if(*c == 'X' || *c == 'x')
		{
			break;
		}
		if(isdigit(*c))
		{
		 	val = *c - '0';
		}
		if(*c >= 'A' && *c <= 'F')
		{
			val = *c - 'A' + 10;
		}
		if(*c >= 'a' && *c <= 'f')
		{
			val = *c - 'a' + 10;
		}
		val *= base;
		hexval += val;
		base *= 16;

	}

	free(rev);
	return hexval;
}


int64_t convert_to_oct(char *number)
{
	int64_t hexval  = 0;
	char *rev = strdup(number);
	reverse(rev);
	int count = 1;
	int base = 1;
	for(char *c = rev; *c; c++)
	{
		int val;

		if(*c >= '0' && *c <= '7')
		{
		 	val = *c - '0';
		}

		val *= base;
		hexval += val;
		base *= 8;

	}

	free(rev);
	return hexval;
}


bool valid_name(char *name)
{
	if(*name == 0)
	{
		return false;
	}
	char *ch = name;
	if(isalpha(*ch))
	{
		ch++;
		for(;*ch; ch++)
		{
			if(!isalnum(*ch) && *ch != '_')
			{
				return false;
			}
		}
		return true;
	}
	return false;
}


