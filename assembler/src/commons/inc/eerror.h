#ifndef __ERROR__
#define __ERROR__


#include <stdlib.h>

typedef enum error_type
{
	UNKNOWN_ERROR,
	TOKEN_ERROR,
	PARSE_ERROR,
	KEY_ERROR,
}error_type_t;





typedef struct error
{
	//error type
	error_type_t ertp;
	//message
	char *msg;
	//extra ptr
	void *extra;
	void (*err_fn )(struct error *err);
}error_t;

typedef void (*err_fn_t )(error_t *err);

void setup_errors(void);


void emit_error(error_type_t, const char *msg, void *extra, err_fn_t fn);

void print_error(int i);
void print_errors(void);


void inline_error(int code, const char *error, const char *filename, size_t line);
char *get_errstr(int code);

#define ERR(code, log) inline_error(code, log, __FILE__, __LINE__)

void print_log(char *log, size_t line, char *filename);

#define LOG(log) print_log(log, __LINE__, __FILE__);

#endif
