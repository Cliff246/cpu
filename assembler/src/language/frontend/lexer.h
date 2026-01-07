#ifndef __LANG_LEXER__
#define __LANG_LEXER__

#include <stdlib.h>

typedef enum
{
    TOK_INVAL, TOK_TOKEN,
	TOK_IDENT, TOK_NUMBER, TOK_STRING, TOK_SEGMENT, TOK_REFERENCE,
    TOK_COLON, TOK_FOLLOWING, TOK_PERCENT,
	TOK_COMMA, TOK_DOT,
	TOK_SYMBOL,
    TOK_NEWLINE, TOK_EOF,
	TOK_COMMENT, TOK_EXCLAIM,
	TOK_QUESTION,
	TOK_EMPTY,
} tok_type_t;

typedef struct toklocale
{
	int row, col, file;
}toklocale_t;

typedef struct
{
    tok_type_t type;
    char *lexeme;
	toklocale_t locale;
} tok_t;

extern tok_t empty_token;


typedef struct
{
	tok_t *toks;
	size_t count, capacity;
	const char *src;
	size_t src_len;

	size_t pos;
	toklocale_t locale;

}lexer_ctx_t;


void free_token(tok_t *ptr);
lexer_ctx_t *create_token_stream(char *src, size_t file_id);
void print_token(tok_t *tok);

char *get_tok_type_string(tok_type_t type);
char *printable_tok(tok_t *tok);

int get_token_row(tok_t *tok);
int get_token_file(tok_t *tok);
int get_token_col(tok_t *tok);
char *get_token_lexme(tok_t *tok);
#endif
