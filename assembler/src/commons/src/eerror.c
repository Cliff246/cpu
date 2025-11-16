
#include "commons.h"
#include <errno.h>
#include "eerror.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


static error_t *_e_list;
static size_t _e_alloc = 10;
static size_t _e_length = 0;

void setup_errors(void)
{
	static bool has_called = false;
	if(has_called)
		return;
	_e_alloc = 16;
	_e_list = (error_t *)CALLOC(_e_alloc, error_t);
	has_called = true;
}



#define MAX_ERROR_ELEMENTS 255
void emit_error(error_type_t type, const char *msg, int count, ...)
{
	va_list va = {0};




	if(_e_length == _e_alloc)
	{
		_e_alloc *= 2;
		_e_list = REALLOC(_e_list, _e_alloc, error_t);
	}

	char *duplicate = strdup(msg);
	_e_list[_e_length].ertp = type;
	_e_list[_e_length].msg = duplicate;

	error_extra_t extra = {0};
	if(count > 0)
	{
		errelm_t buffer[MAX_ERROR_ELEMENTS] = {0};

		if(count >= MAX_ERROR_ELEMENTS)
		{
			perror("to many error elements in one error skipping after 255");
		}
		va_start(va, count);
		for(int i = 0; i < count && i < MAX_ERROR_ELEMENTS; ++i)
		{
			buffer[i] = va_arg(va, errelm_t);
		}
		va_end(va);


		extra.elements = CALLOC(count, errelm_t);
		extra.elements_count = count;
		for(int n = 0; n < count; ++n)
		{
			extra.elements[n] = buffer[n];
		}
	}
	if(count == 0)
	{

	}


	_e_list[_e_length].extra = extra;
	_e_length++;
}

void print_error(int i)
{
	if(i < _e_length && i >= 0)
	{
	  	error_t *err =  &_e_list[i];

		fprintf(stderr,"error: %s msg: %s\n", get_error_type_str(err->ertp), err->msg);
		if(err->extra.elements_count > 0)
		{
			for(int i = 0; i < err->extra.elements_count; ++i)
			{
				errelm_t *elem = &err->extra.elements[i];
				error_element_printer(elem);
			}

		}

	}
}


void print_errors(void)
{
	for(int i = 0; i < _e_length; ++i)
	{
		print_error(i);
	}
}
//not threading safe
char *get_error_type_str(error_type_t code)
{
	#define ERROR_TYPE_STR_BUFFER 1025
	static char buffer[ERROR_TYPE_STR_BUFFER];

	memset(buffer, 0, ERROR_TYPE_STR_BUFFER);


	switch(code)
	{



		default:
			strcpy(buffer, "unknown error");
			break;
	}
	return buffer;
}


static void print_errelm_inval(errelm_t *err)
{
	fprintf(stderr, "unknown element_type\n");
}

static void print_errelm_file(errelm_t *err)
{
	errelm_file_t file = err->elem.file;
	fprintf(stderr, "file name: %s\n", file.name);
}

static void print_errelm_line(errelm_t *err)
{
	errelm_line_t line = err->elem.line;
	fprintf(stderr,"line:%d col: %d\n", line.line, line.column);
}

static void print_errelm_token(errelm_t *err)
{
	errelm_token_t errtok = err->elem.token;
	tok_t *tok = errtok.token;
	print_token(tok);
}

static void print_errelm_string(errelm_t *err)
{

}

typedef void (*print_errelm_fn)(errelm_t *);

print_errelm_fn errelm_fn_print_ary[] =
{
	[ERRELM_INVAL] = print_errelm_inval,
	[ERRELM_FILE] = print_errelm_file,
	[ERRELM_LINE] = print_errelm_line,
	[ERRELM_TOKEN] = print_errelm_token,
	[ERRELM_STRING] = print_errelm_string,
};


void error_element_printer(errelm_t *elm)
{
	fprintf(stderr, "\t");

	if(elm->type >= ARYSIZE(errelm_fn_print_ary))
	{
		print_errelm_inval(elm);
		return;
	}


	print_errelm_fn func = errelm_fn_print_ary[elm->type];


	if(func == NULL)
	{
		print_errelm_inval(elm);
	}

	func(elm);

}






void inline_error(int code, const char *error, const char *filename, size_t line)
{
	fprintf(stderr, "err:\'%s\' at:%s:%lu\n", error, filename, line);
	if(errno != 0)
	{
		perror("errno already set\n");
		exit(1);
	}
	errno = code;
}


char *get_errstr(int code)
{
	//TODO
#define CASE_ERRSTR(code, errst) case code:\
	return errst;
	if(code < 0)
		return "UNKNOWN CODE";
	switch(code)
	{
		CASE_ERRSTR(0, "UNKNOWN ERROR")
		default:
			return "UNKNOWN CODE";
	}
}

void print_log(char *log, size_t line, char *filename)
{
	fprintf(stderr, "%s at %s:%d\n", log, filename, line);
}


errelm_t errelm_create_file_element(errelm_file_t file)
{
	errelm_t element = {.elem.file = file, .type = ERRELM_FILE};
	return element;
}
errelm_t errelm_create_line_element(errelm_line_t line)
{
	errelm_t element = {.elem.line = line, .type = ERRELM_LINE};
	return element;
}
errelm_t errelm_create_token_element(errelm_token_t token)
{
	errelm_t element = {.elem.token = token, .type = ERRELM_TOKEN};
	return element;
}

void escape(int errcode)
{
	print_errors();
	exit(errcode);
}