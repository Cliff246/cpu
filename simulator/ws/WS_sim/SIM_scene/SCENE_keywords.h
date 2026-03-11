#ifndef __SCENE_KEYWORDS_HEADER__
#define __SCENE_KEYWORDS_HEADER__
#include "commons.h"

#include <stdbool.h>


#define SCENE_KEYWORDS_SIZE 6
//name, group, required
#define SCENE_KEYWORDS_LIST(X)\
	X(tag		, 0, 0)		\
	X(port		, 1, 1)		\
	X(id		, 2, 0)		\
	X(name		, 2, 1)		\
	X(handle	, 3, 0)		\
	X(resources	, 3, 0)		\
	X(module	, 3, 0)		\

#define SCENE_KEYWORD_ENUM(X, Y, Z) SCENE_keyword_ ## X,

typedef enum
{
	SCENE_KEYWORDS_LIST(SCENE_KEYWORD_ENUM)
}SCENE_keyword_type_t;

#define SCENE_KEYWORDS_COUNT (0 SCENE_KEYWORDS_LIST(COUNTER3D))

extern char *keywords_strings[SCENE_KEYWORDS_COUNT];
extern int keywords_groups[SCENE_KEYWORDS_COUNT];
extern bool keywords_optional[SCENE_KEYWORDS_COUNT];

#endif