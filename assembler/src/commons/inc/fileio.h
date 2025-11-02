#ifndef __FILEIO__
#define __FILEIO__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ASM_MAX_FILES 128



typedef struct file_descriptor
{
	char *name;
	size_t length;
	char *src;
	FILE *fp;
	int id;

	bool is_closed;
	bool allocated;
	bool has_read;

}file_desc_t;

size_t file_length(FILE *fp);
int file_load(const char *file_name, char **ref);



int allocate_fdesc(const char *file_name);
file_desc_t *get_fdesc(int id);



//points to the files index in target->inputs



char *get_path_from_identifier(int index);
char *get_filename_from_id(int id);


extern file_desc_t descptions[ASM_MAX_FILES];
extern int description_used;




bool validate_path(const char *path);


#endif