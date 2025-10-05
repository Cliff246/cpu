#ifndef __LANG_PARSER__
#define __LANG_PARSER__

#include "lexer.h"


typedef enum 
{
    NODE_PROGRAM, NODE_LABEL, NODE_DIRECTIVE, NODE_INSTR, NODE_EXPR
} node_kind_t;

typedef struct node
{
    node_kind_t kind;
    tok_t *tok;
    struct node **children;
    size_t child_count;
} node_t;


node_t *parse_program(tokstream_t *ts);
node_t *parse_line(tokstream_t *ts);
node_t *parse_instruction(tokstream_t *ts);
node_t *parse_directive(tokstream_t *ts);
node_t *parse_expr(tokstream_t *ts);

#endif
