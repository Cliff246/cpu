#ifndef __LANG_PARSER__
#define __LANG_PARSER__

#include <stdint.h>
#include "lexer.h"


typedef enum
{
    NODE_PROGRAM, NODE_LABEL, NODE_DIRECTIVE, NODE_INSTR, NODE_EXPR
} parse_node_kind_t;

typedef struct parse_node
{
    parse_node_kind_t kind;
    tok_t *tok;
    struct parse_node **children;
    size_t child_count;
} parse_node_t;



typedef struct parser_context
{
	lexer_ctx_t *lexer_ctx_t;
	size_t pos;
}parser_ctx_t;


parse_node_t *get_node_from_ctx_offset(lexer_ctx_t *ctx, int start);

#endif
