
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "flags.h"
#include "assembler.h"
#include "decoder.h"
#include "commons.h"
#include "error.h"
#define ARGUMENTS_COUNT 255


const int table[][1];


size_t file_length(FILE *fp)
{
	int current_seek = ftell(fp);

	fseek(fp, 0, SEEK_END);
	int eof = ftell(fp);
	if(eof == 0)
	{
		perror("ftell failed");
		errno = EACCES;
		return 0;
   	}

	fseek(fp, current_seek, SEEK_SET);
	return eof;
}

int file_load(const char *file_name, char **ref)
{
	FILE *fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		errno = EACCES;
		return -1;
	}
	int length = file_length(fp);
	if(errno == EACCES)
	{
		fclose(fp);
		return -1;
	}
	char *content = (char *)calloc(length + 100, sizeof(char));
	for( int i = 0; i < length; ++i)
	{
		char c = fgetc(fp);

		content[i] = c;


	}

	fclose(fp);
	*ref = content;
	return length;
}




int main(int argc, char *argv[])
{
	setup_errors();
	errno = 0;
	if(argc == 1)
	{
		printf("no argument's provided\n");
		return 1;
	}

	if(argc == 2)
	{
		char *content = NULL;
	   	int length = file_load(argv[1], &content);
		if(errno == EACCES && !content)
		{
			printf("bad file\n");
			return 1;
		}
		else
		{
			printf("assemble\n");
			assemble(content, length, "bin");
			//test(content, length);
		}
		return 0;
	}


	int arguments[ARGUMENTS_COUNT];


	for(int i = 0; i < argc && i < ARGUMENTS_COUNT; ++i)
	{

	}

}
