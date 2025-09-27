
#include "commons.h"
#include <ctype.h>
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



int get_number_type(char *str)
{
	int const posint_type = 1, int_type = 2, hex_type = 3, oct_type = 4, no_type = 0; 
	size_t len = strlen(str);
	if(len > 0)
	{
		
		bool isnumber = true;
		
		if(len == 1)
		{
			return (isdigit(str[0]))? posint_type: no_type;
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
						return no_type;
					
				}
				return posint_type;
			}
			//standard hex number
			else if(c1 == '0' && (c2 == 'x' || c2 == 'X'))
			{
				for(int i = 2; i < len; ++i)
				{
					if((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F') || isdigit(str[i]))
						continue;	
					else
						return no_type;
					
				}	
				return hex_type;
			}
			else if(c1 == '-' && isdigit(c2))
			{
				for(int i = 2; i < len; ++i)
				{
					if(!isdigit(str[i]))
						return no_type
							;
					
				}
				return int_type;
			}
			else if(c1 == '0' && isdigit(c2))
			{
				for(int i = 2; i < len; ++i)
				{
					if(str[i] < '0' || str[i] > 8)
						return no_type;
					
				}
				return oct_type;
			}
			
			
		}	

	}
	return no_type;

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
		
		if(*c >= '0' && *c <= '8')
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
			if(!isalnum(*ch))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}




