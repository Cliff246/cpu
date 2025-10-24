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
	int id;

}file_desc_t;

size_t file_length(FILE *fp);
int file_load(const char *file_name, char **ref);

file_desc_t *get_fdesc(const char *file_name);



#define MAX_SOURCES 1024
//points to the files index in target->inputs

char *get_path_from_identifier(int index);
extern file_desc_t descptions[];

bool validate_path(const char *path);

#endif