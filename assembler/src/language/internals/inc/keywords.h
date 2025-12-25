#ifndef __KEYWORD_HEADER__
#define __KEYWORD_HEADER__

#include <stdlib.h>
#include <stdio.h>

#define KEYWORD_TYPE_SIZE 20
#define KEYWORD_STRING_SIZE 20
typedef int ASM_keytype_t;
typedef int ASM_keyword_id_t;




typedef struct
{
	char alias[KEYWORD_STRING_SIZE];
	char description[100];
	int subpath;
	int path;
}ASM_alias_t;


typedef struct
{
	ASM_keytype_t type;
	ASM_keyword_id_t id;


	ASM_alias_t *aliases;
	size_t aliases;



}ASM_keyword_t;




#endif