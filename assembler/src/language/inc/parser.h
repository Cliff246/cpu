#ifndef __LANG_PARSER__
#define __LANG_PARSER__

#include <stdint.h>
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

node_t *get_node_from_ctx_offset(lexer_ctx_t *ctx, int start);

#endif
