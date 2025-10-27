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
	TOK_COMMENT,
} tok_type_t;

typedef struct locale
{
	size_t row, col, file;
}toklocale_t;

typedef struct
{
    tok_type_t type;
    char *lexeme;
	toklocale_t locale;
} tok_t;


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

#endif
