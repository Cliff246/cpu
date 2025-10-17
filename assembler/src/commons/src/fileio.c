#include "fileio.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "commons.h"

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


int get_content(FILE *fp, char **ref)
{

	int length = file_length(fp);
	if(errno == EACCES)
	{
		fclose(fp);
		return -1;
	}
	char *content = (char *)calloc(length + 100, sizeof(char));
	fread(content, 1, length, fp);
	*ref = content;
}

int file_load(const char *file_name, char **ref)
{
	FILE *fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		errno = EACCES;
		return -1;
	}
	int length = get_content(fp, ref);
	fclose(fp);
	return length;
}




file_desc_t *get_fdesc(const char *file_name)
{
	file_desc_t *desc = CALLOC(1, file_desc_t);
	char *ref = NULL;

	FILE *fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		errno = EACCES;
		exit(1);
	}
	int length = get_content(fp, &ref);
	if(length > 0)
	{
		desc->fp = fp;
		desc->isclosed = true;
		desc->src = ref;
		desc->length = length;
		fclose(fp);

		desc->isclosed = true;
		return desc;
	}
	else
	{
		free(desc);
		perror("failed descriptor");
		return NULL;
	}

}