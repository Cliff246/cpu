#ifndef __ERROR__
#define __ERROR__


#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

typedef enum error_type
{
	UNKNOWN_ERROR,
	TOKEN_ERROR,
	PARSE_ERROR,
	KEY_ERROR,
}error_type_t;


//for each element type there should be a struct in the error element union
//we then stack error elements when writing a error so that it's easy to parse

typedef enum error_element_type
{
	ERRELM_INVAL,
	ERRELM_FILE,
	ERRELM_LINE,
	ERRELM_TOKEN,
}error_element_type_t;


typedef struct errelm_file
{
	char *name;
}errelm_file_t;

typedef struct errelm_line
{
	size_t line;
	size_t column;
}errelm_line_t;



typedef struct errelm_token
{
	tok_t *token;
}errelm_token_t;


typedef struct error_element
{
	error_element_type_t type;
	union
	{
		errelm_file_t file;
		errelm_line_t line;
		errelm_token_t token;
	}elem;
}errelm_t;


errelm_t errelm_create_file_element(errelm_file_t file);
errelm_t errelm_create_line_element(errelm_line_t line);
errelm_t errelm_create_token_element(errelm_token_t token);

typedef struct error_extra
{

	errelm_t *elements;
	size_t elements_count;

}error_extra_t;


typedef struct error
{
	//error type
	error_type_t ertp;
	//message
	char *msg;
	//extra ptr
	error_extra_t extra;
}error_t;

typedef void (*err_fn_t )(error_t *err);

void setup_errors(void);

void error_element_printer(errelm_t *elm);
void emit_error(error_type_t type, const char *msg, int count, ...);

void print_error(int i);
void print_errors(void);


void inline_error(int code, const char *error, const char *filename, size_t line);
char *get_error_type_str(error_type_t code);

#define ERR(code, log) inline_error(code, log, __FILE__, __LINE__)


#endif
