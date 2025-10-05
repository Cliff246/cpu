#include "fileio.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

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
	fread(content, 1, length, fp);
	fclose(fp);
	*ref = content;
	return length;
}