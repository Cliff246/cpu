
#include "loader.h"
#include "commons.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

static void force_close_sourcefile(sourcefile_t *sf)
{


	fclose(sf->file);
	sf->haslength = false;
	sf->length = 0;
	sf->opened = false;
	return;
}

bool get_canopen_sourcefile(sourcefile_t *sf)
{
	if(get_isopen_sourcefile(sf))
	{
		return true;
	}

	FILE* test = fopen(sf->path, "r");
	if(errno != 0)
		assert(0);
	if(test == NULL )
	{


		return false;
	}
	else
	{
		fclose(test);
		return true;
	}

}


bool open_sourcefile(sourcefile_t *sf)
{
	if(get_isopen_sourcefile(sf))
	{
		return false;
	}
	FILE *fp = fopen(sf->path, "rw");

	if(fp == NULL || errno != 0)
	{
		perror("could not open file");
		return false;
	}
	sf->file = fp;
	sf->opened = true;
	return true;
}

bool close_sourcefile(sourcefile_t *sf)
{
	if(!get_isopen_sourcefile(sf))
	{
		return false;
	}

	fclose(sf->file);
	sf->opened = false;
	sf->length = 0;
	sf->haslength = false;
	return true;
}

bool set_cursor_sourcefile(sourcefile_t *sf, int cursor)
{

	if(!get_isopen_sourcefile(sf))
	{
		return false;
	}

	int seek1 = fseek(get_sourcefile_fp(sf), cursor, SEEK_SET);
	if(seek1 != 0)
	{
		perror("could not seek file\n");
		return false;
	}

	return true;

}



int get_cursor_sourcefile(sourcefile_t *sf)
{
	if(!get_isopen_sourcefile(sf))
	{
		return -1;
	}
	int result = ftell(get_sourcefile_fp(sf));
	if(result < 0)
	{
		perror("could not get currsor");
		return -1;
	}

	return result;

}
void reset_cursor_sourcefile(sourcefile_t *sf)
{
	if(!get_isopen_sourcefile(sf))
	{
		return;
	}

	int seek1 = fseek(get_sourcefile_fp(sf), 0, SEEK_SET);
	if(seek1 != 0)
	{
		perror("could not seek file\n");
		return;
	}
	return;
}


char *read_sourcefile(sourcefile_t *sf, int length)
{
	if(!get_isopen_sourcefile(sf) || length < 0)
	{
		perror("read with invalid paramaters");

		return NULL;
	}

	if(sf->haslength)
	{
		if(sf->length < length + get_cursor_sourcefile(sf))
		{
			perror("desired read length + cursor less than file length");
			return NULL;
		}
	}

	char *content = calloc(length + 1, sizeof(char));

	if(content == NULL)
	{
		perror("could not alloacte enought memory\n");
		return NULL;
	}

	const int retcode =  fread(content, sizeof(char), length, get_sourcefile_fp(sf));
	if(retcode == length)
	{
		return content;
	}
	else
	{
		if(feof(get_sourcefile_fp(sf)))
		{
			perror("error with source file: unexpected end of file");
		}
		else if (ferror(get_sourcefile_fp(sf)))
		{
			perror("error with source file");

		}
		force_close_sourcefile(sf);
		return NULL;
	}
}

char *readline_sourcefile(sourcefile_t *sf)
{

	if(!get_isopen_sourcefile(sf))
	{
		perror("read with invalid paramaters");

		return NULL;
	}

	size_t guess = 20;

	char *content = calloc(guess + 1, sizeof(char));
	if(content == NULL)
	{
		perror("could not alloacte enought memory\n");
		return NULL;
	}


	int current = 0;
	FILE *fp = get_sourcefile_fp(sf);
	while(1)
	{
		char ch = fgetc(fp);
		if(!feof(fp) && ch != 10)
		{
			//printf("current %c\n", ch);
			if(current >= guess)
			{
				guess *= 2;
				content = realloc_safe(content, guess, sizeof(char));
			}
			content[current++] = (char)ch;
		}
		else
		{
			break;
		}

	}



	return content;
}


void write_sourcefile(sourcefile_t *sf, char *content)
{

	if(!get_isopen_sourcefile(sf) || content == NULL)
	{
		perror("write with invalid paramaters");
		return;
	}

	int content_len = strlen(content);

	if(sf->haslength)
	{
		sf->haslength = false;
		sf->length = 0;
	}

	fwrite(content, sizeof(char), content_len, get_sourcefile_fp(sf));

	sf->modified = true;
}

void add_newline_sourcefile(sourcefile_t *sf)
{
	if(!get_isopen_sourcefile(sf))
	{
		perror("write with invalid paramaters");
		return;
	}


	if(sf->haslength)
	{
		sf->haslength = false;
		sf->length = 0;
	}
	fputc('\n', get_sourcefile_fp(sf));

	sf->modified = true;
}

bool get_isopen_sourcefile(sourcefile_t *sf)
{
	return sf->opened;
}


FILE *get_sourcefile_fp(sourcefile_t *sf)
{
	if(sf->file == NULL || sf->opened == false)
	{
		fprintf(stderr,"cannot get fp if file is closed: %s", sf->path);
		exit(1);
	}
	return sf->file;
}





int get_sourcefile_length(sourcefile_t *sf)
{
	if(!get_isopen_sourcefile(sf))
	{
		perror("read with invalid paramaters");

		return -1;
	}

	if(sf->modified == false)
	{
		if(sf->haslength == true)
		{
			return sf->length;
		}

	}
	int length = 0;

	int cursor = get_cursor_sourcefile(sf);
	reset_cursor_sourcefile(sf);
	int seek = fseek(get_sourcefile_fp(sf), 0, SEEK_END);
	if(seek < 0)
	{

		force_close_sourcefile(sf);
		return -2;
	}
	length = ftell(get_sourcefile_fp(sf));

	//printf("%d\n", seek);

	if(!set_cursor_sourcefile(sf, cursor))
	{

		return false;
	}


	sf->length = length;
	sf->haslength = true;
	//printf("cursor\n");

	return length;
}


sourcefile_t *create_sourcefile(char *path)
{
	sourcefile_t *sf = calloc(1, sizeof(sourcefile_t));
	char *dup = strdup(path);
	if(!sf || !dup)
	{
		perror("source file could not be created due to memory");
		exit(EXIT_FAILURE);
	}

	sf->path = dup;
	return sf;
}

bool get_isend_sourcefile(sourcefile_t *sf)
{
	return (bool)feof(get_sourcefile_fp(sf));
}

void free_sourcefile(sourcefile_t *sf)
{
	close_sourcefile(sf);
	free(sf->path);
	free(sf);
}
