#ifndef __FILEIO__
#define __FILEIO__

#include <stdlib.h>
#include <stdio.h>
size_t file_length(FILE *fp);
int file_load(const char *file_name, char **ref);

#endif