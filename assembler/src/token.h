#ifndef __TOKEN__
#define __TOKEN__

#include <stdlib.h>

typedef enum 
{
    TOK_INVAL,
    TOK_EOF,

    TOK_TEXT,         // names: 
    TOK_NUMBER,       // 123, 0xFF
	TOK_REFERENCE,	  // reference:
    TOK_DIRECTIVE,    // .text, .realloc
    TOK_REGISTER,     // x0, zero
    TOK_OP_WORD,	  // alu
	TOK_SUBOP_WORD,	  // 
	TOK_COLON,        // :
    TOK_COMMA,        // ,
	TOK_DOT,		  // .
	
} toktype_t;

typedef struct token
{
	char *lexme;
	toktype_t type;
	size_t line, column, file;
}tok_t;


#endif
