#ifndef __WS_IO_PARSER_HEADER__
#define __WS_IO_PARSER_HEADER__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "token.h"


//WS_IO_node
//node of parse tree 
typedef struct WS_IO_node
{
	tok_t *tok;	
	size_t size;
	struct WS_IO_node *nodes;
}IO_node_t;

typedef struct WS_IO_ptree
{
	size_t tree;
	IO_node_t *head;

}IO_ptree_t;

IO_ptree_t *IO_init_ptree(toklex_t *);

#endif
