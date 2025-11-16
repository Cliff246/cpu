
#include "grader.h"

#include <stdlib.h>
#include <errno.h>




grade_t *create_grade()
{
	grade_t *grade = calloc(1, sizeof(grade_t));
	if(!grade)
	{
		perror("calloc failed in create grade\n");
		exit(EXIT_FAILURE);
	}
	grade->type = GRADE_INVAL;
	return grade;
}