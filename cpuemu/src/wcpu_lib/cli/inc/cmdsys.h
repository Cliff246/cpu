#ifndef __CMD_SYS__
#define __CMD_SYS__

#include "token.h"
#include <stdlib.h>


typedef enum pnodetype
{
	PN_START,
	PN_OP,
	PN_NUM,
	PN_ADD,
	PN_SUB,
	PN_DIV,
	PN_MUL,
	PN_EMPTY,
}pnode_type_t;

typedef struct pnode
{
	struct pnode **nodes;
	size_t length;
	tok_t *token;
	pnode_type_t type;
}pnode_t;


typedef struct pcontext
{
	
	toklex_t *lex;
	size_t index;
}pcontext_t;

typedef struct cmdline
{
	pnode_t *head;
	toklex_t *lex;
}cmdline_t;

pnode_t *evaluate_lex(toklex_t *lex);


#endif
