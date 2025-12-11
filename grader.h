#ifndef __GRADER_HEADER__
#define __GRADER_HEADER__

#include "loader.h"
#include "rubrik.h"

#include <stdint.h>


typedef enum grade_type
{
	GRADE_INVAL,
	GRADE_MEMORY,
	GRADE_REGISTER,
}grade_type_t;

typedef struct grade_register
{
	int reg;
	int64_t expects;
}grade_register_t;

typedef struct grade_memory
{
	int start, stop;
	int64_t *expects;
	size_t count;
}grade_memory_t;

typedef struct grade
{
	grade_type_t type;

	union
	{
		grade_memory_t tamem;
		grade_register_t tareg;
	}asrt;

}grade_t, *p_grade_t;


grade_t *create_grade();



typedef struct grader
{
	rubrik_t *rubrik;

	p_grade_t *grades;
	int length;


}grader_t;

#endif


