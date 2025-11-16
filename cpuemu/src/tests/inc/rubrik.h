

#ifndef __RUBRIK_HEADER__
#define __RUBRIK_HEADER__

#include "loader.h"



typedef struct question
{
	char *question_line;
}question_t;

typedef struct answer
{
	char *answer_line;
}answer_t;

typedef struct qaelem
{
	question_t question;
	answer_t answer;



}qaelem_t;

typedef struct rubrik
{



	qaelem_t **qa_elems;
	int elems_size, elems_alloc;


	sourcefile_t *srcfile;



	bool generate_rubrik;
	bool parsed_rubrik;

}rubrik_t;


bool add_qaelem_rubric(rubrik_t *rubric, qaelem_t *elm);

bool read_rubric(rubrik_t *rubrik);


rubrik_t *create_rubric(char *path);

#endif