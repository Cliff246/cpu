
#include "commons.h"
#include <errno.h>
#include "eerror.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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



void emit_error(error_type_t type, const char *msg, void *extra, err_fn_t fn)
{

	if(_e_length == _e_alloc)
	{
		_e_alloc *= 2;
		_e_list = REALLOC(_e_list, _e_alloc, error_t);
	}

	char *duplicate = strdup(msg);
	_e_list[_e_length].ertp = type;
	_e_list[_e_length].msg = duplicate;

	_e_list[_e_length].extra = extra;
	_e_list[_e_length].err_fn = fn;
	_e_length++;
}

void print_error(int i)
{
	if(i < _e_length && i >= 0)
	{
		if(_e_list[i].err_fn == NULL)
		{

			fprintf(stderr, "type:%d msg:\'%s\'\n\terror str: \'%s\'\n", _e_list[i].ertp, _e_list[i].msg, get_errstr(_e_list[i].ertp));
		}
		else
		{
			//calls custom error;
			_e_list[i].err_fn(&_e_list[i]);
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




//takes error with tok_t as error
void error_token_error(error_t *err)
{
	tok_t *token = err->extra;

	//TODO replace file with a string look up for name
	fprintf(stderr,"error at: f:%d line:%s col:%d\n", token->locale.file, token->locale.row, token->locale.col);
	fprintf(stderr, "type:%d msg:\'%s\'\n", err->ertp, err->msg);

}

void error_unknown_error(error_t *err)
{

}



err_fn_t error_class_allocate(error_type_t type)
{

	err_fn_t funcs[] = {
		error_unknown_error,
		error_token_error,
	};

	return *funcs[type];

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