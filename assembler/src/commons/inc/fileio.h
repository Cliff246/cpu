#ifndef __FILEIO__
#define __FILEIO__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct file_descriptor
{
	char *name;
	size_t length;
	char *src;
	FILE *fp;
	bool isclosed;

}file_desc_t;

size_t file_length(FILE *fp);
int file_load(const char *file_name, char **ref);

file_desc_t *get_fdesc(const char *file_name);

#endif