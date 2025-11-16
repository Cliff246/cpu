#ifndef __TEST_HEADER__
#define __TEST_HEADER__

#include "loader.h"
#include "rubrik.h"
#include "exporter.h"
#include "tester.h"
#include "grader.h"

typedef struct test
{

	rubrik_t *rubrik;

	grader_t *graders;

	tester_t *tester;
}test_t;


#endif