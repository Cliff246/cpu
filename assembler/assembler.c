
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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

char *file_load(const char *file_name)
{
	FILE *fp = fopen(file_name, "rb");
	if(fp == NULL)
	{
		errno = EACCES;
		return NULL;
	}
	int length = file_length(fp);
	if(errno == EACCES)
	{
		fclose(fp);
		return NULL;
	}
	char *content = (char *)calloc(length + 100, sizeof(char));
	for( int i = 0; i < length; ++i)
	{
		char c = fgetc(fp);

		content[i] = c;


	}
	fclose(fp);
	return content;
}



int main(int argc, char *argv[])
{

	errno = 0;
	if(argc == 1)
	{
		printf("no argument's provided\n");
		return 1;
	}

	if(argc == 2)
	{
		char *content = file_load(argv[1]);
		if(errno == EACCES && !content)
		{
			printf("bad file\n");
			return 1;
		}
		else
		{
			printf("%s\n", content);
		}
		return 0;
	}


	int arguments[ARGUMENTS_COUNT];


	for(int i = 0; i < argc && i < ARGUMENTS_COUNT; ++i)
	{

	}

}
