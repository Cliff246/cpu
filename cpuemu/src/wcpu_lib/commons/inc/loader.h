#ifndef __LOADER_HEADER__
#define __LOADER_HEADER__


#include <stdio.h>
#include <stdbool.h>




typedef struct sourcefile
{
	char *path;

	FILE *file;
	int length;

	bool opened;


	bool haslength;

	bool modified;

}sourcefile_t;


bool get_canopen_sourcefile(sourcefile_t *sf);

bool open_sourcefile(sourcefile_t *sf);

bool close_sourcefile(sourcefile_t *sf);

char *read_sourcefile(sourcefile_t *sf, int length);
void write_sourcefile(sourcefile_t *sf, char *content);
void add_newline_sourcefile(sourcefile_t *sf);
bool get_isopen_sourcefile(sourcefile_t *sf);

bool set_cursor_sourcefile(sourcefile_t *sf, int cursor);
int get_cursor_sourcefile(sourcefile_t *sf);
void reset_cursor_sourcefile(sourcefile_t *sf);

FILE *get_sourcefile_fp(sourcefile_t *sf);
int get_sourcefile_length(sourcefile_t *sf);



sourcefile_t *create_sourcefile(char *path);



#endif