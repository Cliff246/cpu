#ifndef __WS_IO_PARSER_HEADER__
#define __WS_IO_PARSER_HEADER__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "token.h"

#define IO_NODE_TYPE_LIST(X)\
	X(NONE)					\
	X(START)				\
	X(HEADER)				\
	X(BODY)					\
	X(SET)					\
	X(KEY)					\
	X(VALUE)				\
	X(LIST)					\
	X(BRACKET)				\

#define IO_NODE_NAME(X) IO_PNODE_ ## X
#define IO_NODE_ENUM(X) IO_NODE_NAME(X),


typedef enum WS_IO_pnode_type
{
	IO_NODE_TYPE_LIST(IO_NODE_ENUM)
}IO_pnode_type_t;

//WS_IO_node
//node of parse tree
typedef struct WS_IO_pnode
{
	IO_pnode_type_t type;
	uint32_t size;
	tok_t *token;
	struct WS_IO_pnode **nodes;
}IO_pnode_t;

typedef struct WS_IO_ptree
{
	uint64_t index;
	toklex_t *lex;
	IO_pnode_t *head;

}IO_ptree_t;


static IO_pnode_t *IO_pnode_set(IO_ptree_t *tree);
static IO_pnode_t *IO_pnode_body(IO_ptree_t *tree);
static IO_pnode_t *IO_pnode_header(IO_ptree_t *tree);

void IO_pnode_print(IO_pnode_t *node, int depth);
IO_ptree_t *IO_ptree_create(toklex_t *tl);
bool IO_ptree_parse(IO_ptree_t *tree);
IO_pnode_t *IO_pnode_create(tok_t *tok, IO_pnode_type_t type);
void IO_pnode_append(IO_pnode_t *parent, IO_pnode_t *child);
tok_t *IO_ptree_next_tok(IO_ptree_t *ctx);

tok_t *IO_ptree_peek_tok(IO_ptree_t *tree);

tok_t *IO_ptree_expect_tok(IO_ptree_t *tree, tok_type_t type);


#endif
