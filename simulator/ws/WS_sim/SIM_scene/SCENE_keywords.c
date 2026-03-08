#include "SCENE_keywords.h"

#include <stdbool.h>

#define SCENE_KEYWORDS_LIST_STRINGS(X, Y, Z) [SCENE_keyword_ ## X] = #X,
#define SCENE_KEYWORDS_LIST_GROUPS(X, Y, Z) [SCENE_keyword_ ## X] = Y,
#define SCENE_KEYWORDS_LIST_OPTIONAL(X, Y, Z) [SCENE_keyword_ ## X] = Z,
char *keywords_strings[SCENE_KEYWORDS_COUNT] =
{
	SCENE_KEYWORDS_LIST(SCENE_KEYWORDS_LIST_STRINGS)
};

int keywords_groups[SCENE_KEYWORDS_COUNT] =
{
	SCENE_KEYWORDS_LIST(SCENE_KEYWORDS_LIST_GROUPS)

};

bool keywords_optional[SCENE_KEYWORDS_COUNT] =
{
	SCENE_KEYWORDS_LIST(SCENE_KEYWORDS_LIST_OPTIONAL)

};