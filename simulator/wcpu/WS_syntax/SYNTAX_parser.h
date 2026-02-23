#ifndef __SYNTAX_PARSER_HEADER__
#define __SYNTAX_PARSER_HEADER__


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "token.h"

/*


	KEYWORD = (tok, device, resource, name, hnd)
	NAME = (keyword(name))
	INIT = MONO | LIST | MAP
*/
#define SYNTAX_NODE_TYPE_LIST(X)\
	X(NONE)					\
	X(ENTRY)				\
	X(KEYWORD)				\
	X(NAME)					\
	X(NONAME)				\
	X(MONO)					\
	X(LIST)					\
	X(MAP)					\
	X(VALUE)				\


#define SYNTAX_NODE_NAME(X) SYNTAX_PNODE_ ## X
#define SYNTAX_NODE_ENUM(X) SYNTAX_NODE_NAME(X),


typedef enum WS_SYNTAX_pnode_type
{
	SYNTAX_NODE_TYPE_LIST(SYNTAX_NODE_ENUM)
}SYNTAX_pnode_type_t;

//WS_SYNTAX_node
//node of parse tree
typedef struct WS_SYNTAX_pnode
{
	SYNTAX_pnode_type_t type;
	uint32_t size;
	uint32_t alloc;
	tok_t *token;
	struct WS_SYNTAX_pnode **nodes;
}SYNTAX_pnode_t;

typedef struct WS_SYNTAX_ptree
{
	uint64_t index;
	toklex_t *lex;
	SYNTAX_pnode_t *head;
}SYNTAX_ptree_t;


static SYNTAX_pnode_t *SYNTAX_ptree_value(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_mono(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_list(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_map(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_init(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_name(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_noname(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_keyword(SYNTAX_ptree_t *tree);
static SYNTAX_pnode_t *SYNTAX_ptree_entry(SYNTAX_ptree_t *tree);


void SYNTAX_pnode_print(SYNTAX_pnode_t *node, int depth);
SYNTAX_ptree_t *SYNTAX_ptree_create(toklex_t *tl);
bool SYNTAX_ptree_parse(SYNTAX_ptree_t *tree);
SYNTAX_pnode_t *SYNTAX_pnode_create(tok_t *tok, SYNTAX_pnode_type_t type);
void SYNTAX_pnode_append(SYNTAX_pnode_t *parent, SYNTAX_pnode_t *child);
void SYNTAX_ptree_next_tok(SYNTAX_ptree_t *ctx);
tok_t *SYNTAX_ptree_peek_tok(SYNTAX_ptree_t *tree);

tok_t *SYNTAX_ptree_expect_tok(SYNTAX_ptree_t *tree, tok_type_t type);
void SYNTAX_pnode_free(SYNTAX_pnode_t *node);

void SYNTAX_ptree_free(SYNTAX_ptree_t *tree);

#endif

