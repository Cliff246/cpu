#ifndef __TOKEN__
#define __TOKEN__

#include <stdlib.h>
#include <stdbool.h>

typedef enum token_type
{
	TOK_NONE,
	TOK_END,
	TOK_WORD,
	TOK_HEX,
	TOK_INT,
	TOK_OCT,
	TOK_BIN,

	TOK_OP,
	TOK_BRACKET,
	TOK_STRING,
	TOK_COMMA,
	TOK_DOT,




}tok_type_t;

typedef struct token
{
	tok_type_t type;
	char *token;
}tok_t;

typedef struct token_lexer
{
	tok_t *tokens;
	size_t tcount;
	char *string;
	int index;
	size_t slen;


}toklex_t;

extern tok_t empty_tok;

toklex_t *lex_string(const char *string);

void print_toklex(toklex_t *tl);

void print_tok(tok_t *token);

void reset_toklex(toklex_t *tl);
tok_t *get_toklex(toklex_t *tl, int index);
tok_t *peek_toklex(toklex_t *tl);
bool expect_toklex(toklex_t *tl, tok_type_t type);


void free_tokstring(tok_t *token);
void free_toklex(toklex_t *toklex);


#endif
