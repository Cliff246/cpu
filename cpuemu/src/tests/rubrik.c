#include "rubrik.h"
#include "loader.h"
#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

rubrik_t *create_rubric(char *path)
{

	rubrik_t *rb = calloc(1, sizeof(rubrik_t));
	if(rb == NULL)
	{
		perror("calloc error");
		exit(EXIT_FAILURE);
	}


	rb->srcfile = create_sourcefile(path);
	if(!get_canopen_sourcefile(rb->srcfile))
	{
		fprintf(stderr, "cannot open file: %s", path);
		exit(EXIT_FAILURE);
	}

	rb->generate_rubrik = false;
	return rb;
}


static bool parse_rubrik(rubrik_t *rb, char *content)
{



	return true;
}



bool read_rubric(rubrik_t *rubrik)
{


	bool open = open_sourcefile(rubrik->srcfile);
	if(open == false)
	{
		fprintf(stderr, "cannot open rubrik: %s", rubrik->srcfile->path);
		exit(EXIT_FAILURE);

	}
	int length = get_sourcefile_length(rubrik->srcfile);

	char *content = read_sourcefile(rubrik->srcfile, length);

	if(!parse_rubrik(rubrik, content))
	{
		fprintf(stderr,"could not parse rubrik: %s", rubrik->srcfile->path);
		exit(EXIT_FAILURE);
	}


}


bool add_qaelem_rubric(rubrik_t *rubric, qaelem_t *elm)
{
	if(rubric->elems_alloc <= 0)
	{
		rubric->elems_alloc = 10;

		rubric->qa_elems = calloc(rubric->elems_alloc, sizeof(qaelem_t *));

	}
	if(rubric->elems_alloc < rubric->elems_size)
	{
		rubric->elems_alloc *= 2;
		rubric->qa_elems = realloc_safe(rubric->qa_elems, rubric->elems_alloc, sizeof(qaelem_t *));
	}

	rubric->qa_elems[rubric->elems_size++] = elm;
	return true;
}