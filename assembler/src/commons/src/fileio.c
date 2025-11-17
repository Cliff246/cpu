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


int allocate_fdesc(const char *file_name)
{
	if(description_used >= ASM_MAX_FILES)
	{
		printf("too many files used %d\n ", description_used);
		exit(1);
	}
	file_desc_t *desc = &descptions[description_used];

	desc->has_read = false;
	desc->is_closed = true;
	desc->name = (char *)file_name;
	desc->allocated = true;
	desc->id = description_used;

	int used = description_used;
	description_used++;
	return used;
}

file_desc_t *get_fdesc(int id)
{




	file_desc_t *desc = &descptions[id];


	char *file_name = desc->name;
	char *ref = NULL;

	FILE *fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		errno = EACCES;
		exit(1);
	}
	desc->is_closed = false;

	int length = get_content(fp, &ref);
	if(length >= 0)
	{
		desc->fp = fp;
		desc->src = ref;
		desc->length = length;
		fclose(fp);

		desc->is_closed = true;
		desc->has_read = true;
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
	printf("%s\n", path);
	return true;
}


file_desc_t descptions[ASM_MAX_FILES] = {0};
int description_used = 0;


char *get_path_from_identifier(int index)
{

	if(index >= 0)
	{
		if(index < ASM_MAX_FILES)
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

char *get_filename_from_id(int id)
{
	return get_path_from_identifier(id);
}