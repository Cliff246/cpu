
#include "common.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
bool logger_set = true;

void logit(char *fmt, size_t line, char *filename, ...)
{
	if(logger_set == false)
	{
		return;
	}

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