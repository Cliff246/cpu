#ifndef __LANG_PARSER__
#define __LANG_PARSER__

#include <stdint.h>
#include "lexer.h"


typedef enum
{
    NODE_PROGRAM, NODE_REFERENCE, NODE_SEGMENT, NODE_INSTR, NODE_EXPR,
	NODE_METAOP,
	NODE_LITERAL,
	NODE_PATH,
	NODE_SUBPATH,
	NODE_RD,
	NODE_RS1,
	NODE_RS2
} parse_node_kind_t;

typedef struct parse_node
{
    parse_node_kind_t kind;
    tok_t *tok;
    struct parse_node **children;
    size_t child_count;
} parse_node_t;

void free_parse_node(parse_node_t *ptr);


typedef struct parser_context
{
	lexer_ctx_t *lex_ctx;
	size_t pos;
}parser_ctx_t;

void print_depth(parse_node_t *node, int depth);

parser_ctx_t *create_context(lexer_ctx_t *ctx);

parse_node_t *parse_program(parser_ctx_t *p);

#endif
