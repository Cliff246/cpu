#ifndef __LINKER__
#define __LINKER__

#include "inter.h"
#include <stdbool.h>


typedef struct linkflags
{

	bool does_parse;
	bool does_lex;

	int code_segments;
	int text_segments;
	int config_segments;
	int data_segments;

}linkflags_t;

typedef struct link
{

	icontext_t *ctx;

	linkflags_t flags;
}link_t;


typedef struct global
{
	char *key;
	link_t *src;


}global_t;

typedef struct linker
{
	link_t *links;
	int length;


	p_hashtable_t globals;

}linker_t;


void get_globals_from_link(linker_t *lctx, link_t *link);

void add_link_to_linker(linker_t *lctx, icontext_t *ctx);


linker_t *create_linker(void);

#endif