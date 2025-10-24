#include "fileio.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "eerror.h"
#include <stdio.h>
#include "string.h"
#include "commons.h"
#include <sys/stat.h>

size_t file_length(FILE *fp)
{
	int current_seek = ftell(fp);

	int seek1 = fseek(fp, 0, SEEK_END);
	int eof = ftell(fp);
	if(eof <= 0)
	{
		perror("ftell failed");
		errno = EACCES;
		return 0;
   	}

	int seek2 = fseek(fp, current_seek, SEEK_SET);
	return (size_t)eof;
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
	return length;
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
	static int id_index = 0;



	file_desc_t *desc = &descptions[id_index];
	desc->id = id_index;
	id_index++;
	char *ref = NULL;

	FILE *fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		errno = EACCES;
		exit(1);
	}
	int length = get_content(fp, &ref);
	if(length >= 0)
	{
		desc->fp = fp;
		desc->isclosed = true;
		desc->src = ref;
		desc->name = file_name;
		desc->length = length;
		fclose(fp);

		desc->isclosed = true;
		return desc;
	}
	else if(errno == EACCES)
	{
		fclose(fp);
		fprintf(stderr, "%d: %s\n", errno, strerror(errno));
		perror("failed descriptor");
		return NULL;
	}
	else
	{
		fprintf(stderr, "%d: %s\n", errno, strerror(errno));
		perror("failed descriptor");
		exit(1);
	}

}



bool validate_path(const char *path)
{
	struct stat pstat = {0};
  	int value = stat(path, &pstat);
	if(value <= -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}


file_desc_t descptions[MAX_SOURCES] = {0};


char *get_path_from_identifier(int index)
{

	if(index >= 0)
	{
		if(index < MAX_SOURCES)
		{
			if(descptions[index].id >= 0)
				return descptions[index].name;
			else
				return "NOFILE";
		}
		return "OUT OF RANGE";

	}
	else
	{
		return "NO FILE";
	}
}