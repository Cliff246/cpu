
#include "commons.h"
#include "eerror.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>
#include "arguments.h"


void *safe_calloc(size_t size, size_t type_size)
{
	//printf("calloc(%lu,%lu)\n", size, type_size);

	void *ptr = calloc(size, type_size);
	if(!ptr)
	{
		ERR(ENOMEM, "calloc failed");
		exit(1);
	}
	return ptr;

}

void *safe_realloc(void *ptr, size_t size, int type_size)
{
	//printf("realloc %d %d\n", size, type_size);
	if(ptr == NULL)
	{
		ERR(EINVAL, "realloc argument was NULL");
		return NULL;
	}
	else
	{
		//printf("realloc: %zu\n", size * type_size);
		//printf("%p\n", ptr);
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
#undef BUFSIZE
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


typedef enum fmt_type
{
	FMT_NONE,
	FMT_STRING,
	FMT_SINT,
	FMT_UINT,
	FMT_CHAR,
	FMT_HEX,
	FMT_PTR,
	FMT_FLOAT
}fmt_type_t;

void logit(char *fmt, size_t line, char *filename, ...)
{

	if(target.debug_enabled != true)
		return;

	va_list va = {0};


#define LOG_MAX_LENGTH 2048
#define FMT_MAX_LENGTH 100

	char buffer[LOG_MAX_LENGTH + 1] = {0};
	char format[FMT_MAX_LENGTH + 1] = {0};
	int format_index = 0;

	size_t biter = 0;
	bool fmt_char = false;

	bool fmt_done = false;
	union
	{
		char *str;
		char ch;
		int64_t sint;
		uint64_t uint;
		float decimal;
		void *ptr;
	} types;
	fmt_type_t cur_fmt = FMT_NONE;
	va_start(va, filename);
	for(char *c = fmt; *c; c++)
	{

		if(biter >= LOG_MAX_LENGTH)
		{
			perror("log string is too long\n");
			exit(EXIT_FAILURE);
		}

		if(*c == '%')
		{
			if(fmt_char)
			{
				fmt_char = false;
				fmt_done = false;
				cur_fmt = FMT_NONE;

				format_index = 0;
				memset(format, 0, 100);
				buffer[biter++] = *c;
			}
			else
			{
				format[format_index++] = *c;
				fmt_char = true;
			}

		}
		else if(fmt_char)
		{
			if(format_index >= FMT_MAX_LENGTH)
			{
				perror("format over sized");
				exit(EXIT_FAILURE);
			}

			switch(*c)
			{
				case 'd':
					cur_fmt = FMT_SINT;
					types.sint = va_arg(va, int64_t);
					fmt_done = true;
					break;
				case 'u':
					cur_fmt = FMT_UINT;

					types.uint = va_arg(va, uint64_t);
					fmt_done = true;

					break;
				case 'x':
					cur_fmt = FMT_HEX;

					types.uint = va_arg(va, uint64_t);
					fmt_done = true;

					break;
				case 's':
					cur_fmt = FMT_STRING;

					types.str = va_arg(va, char *);
					//printf("try\n");
					fmt_done = true;

					break;
				case 'f':
					cur_fmt = FMT_FLOAT;

					types.decimal = va_arg(va, double);
					fmt_done = true;

					break;
				case 'F':
					cur_fmt = FMT_FLOAT;

					types.decimal = va_arg(va, double);
					fmt_done = true;

					break;
				case 'c':
					cur_fmt = FMT_CHAR;
					types.ch = va_arg(va, char);
					fmt_done = true;

					break;
				case 'p':
					cur_fmt = FMT_PTR;
					types.ptr = va_arg(va, void *);
					fmt_done = true;
					break;

				default:
					break;
			}
			format[format_index++] = *c;

			//evaluate the format
			if(fmt_done)
			{


				char temp_buffer[1000] = {0};
				switch(cur_fmt)
				{
					case FMT_CHAR:
						sprintf(temp_buffer, format, (char)types.ch);
						break;
					case FMT_STRING:
						sprintf(temp_buffer, format, (char *)types.str);
						break;
					case FMT_HEX:
						sprintf(temp_buffer, format, (uint64_t)types.uint);
						break;
					case FMT_SINT:
						sprintf(temp_buffer, format, (uint64_t)types.sint);
						break;
					case FMT_UINT:
						sprintf(temp_buffer, format, (uint64_t)types.uint);
						break;
					case FMT_PTR:
						sprintf(temp_buffer, format, (void *)types.ptr);
						break;
					case FMT_FLOAT:
						sprintf(temp_buffer, format, (float)types.decimal);
						break;
					default:
						break;
				}
				for(int ti = 0; biter < LOG_MAX_LENGTH; biter++, ti++)
				{

					if(temp_buffer[ti] == 0 )
					{
						break;
					}
					buffer[biter] = temp_buffer[ti];

				}


				format_index = 0;
				cur_fmt = FMT_NONE;
				fmt_char = false;
				fmt_done = false;
				memset(format, 0, 100);
			}
		}
		else
		{
			buffer[biter++] = *c;
		}

	}
	va_end(va);

	fprintf(stdout, "<%s:%d> %s",filename, line, buffer);
}

int remove_quotes(char *string, char *buffer, size_t length)
{
	memset(buffer, 0, length);
	int current = 0, quotes = 0;
	for(char *c = string; *c; c++)
	{
		if(*c != '\"' && *c != '\'')
		{
			quotes++;
			if(current >= length)
			{
				buffer[current] = 0;
				return quotes ;
			}
			buffer[current++] = *c;

		}
	}
	return quotes;
}