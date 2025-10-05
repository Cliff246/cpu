#ifndef __LANG_LEXER__
#define __LANG_LEXER__

#include <stdlib.h>

typedef enum 
{
    TOK_IDENT, TOK_NUMBER, TOK_STRING, TOK_DIRECTIVE,
    TOK_COLON, TOK_COMMA,
    TOK_PLUS, TOK_MINUS, TOK_STAR, TOK_SLASH,
    TOK_LPAREN, TOK_RPAREN,
    TOK_NEWLINE, TOK_EOF
} tok_type_t;

typedef struct 
{
    tok_type_t type;
    char *lexeme;
    size_t line, col, file;

} tok_t;

typedef struct tokstream
{
	tok_t *toks;
	size_t len;

}tokstream_t;

#endif
