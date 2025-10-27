#include "lexer.h"
#include "commons.h"
#include "strtools.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include "arguments.h"

void free_token(tok_t *ptr)
{
	//i tried
	if(ptr->lexeme < (char *)basic_strings || ptr->lexeme >= (char *)basic_strings + (256 * 2 * sizeof(char)))
		free(ptr->lexeme);
	free(ptr);
}

void lexer_emit(lexer_ctx_t *ctx, tok_type_t type, char *lexme)
{
	const size_t default_alloc = 100;
	if (ctx->toks == NULL)
	{
		ctx->toks = CALLOC(default_alloc, tok_t);
		ctx->capacity = default_alloc;
		ctx->count = 0;
	}
	if (ctx->count >= ctx->capacity)
	{
		ctx->capacity *= 2;
		ctx->toks = REALLOC(ctx->toks, ctx->capacity, tok_t);
	}
	//this is where lexer tokens are created
	ctx->toks[ctx->count].lexeme = lexme;
	ctx->toks[ctx->count].locale = ctx->locale;
	ctx->toks[ctx->count].type = type;
	//print_token(&ctx->toks[ctx->count]);

	ctx->count++;
}

#define EMIT(type, lexme) lexer_emit(ctx, type, lexme)

char lexer_get_char(lexer_ctx_t *ctx)
{
	//printf("char: %c\n", ctx->src[ctx->pos]);
	if (ctx->src[ctx->pos] == EOF)
	{
		return 0;
	}
	else
	{
		return ctx->src[ctx->pos];
	}
}
#define GETCHAR lexer_get_char(ctx)

char lexer_peek(lexer_ctx_t *ctx)
{
	if (ctx->src_len <= ctx->pos + 1)
	{
		return 0;
	}
	else
	{
		return ctx->src[ctx->pos + 1];
	}
}
#define PEEK lexer_peek(ctx)

int lexer_advance(lexer_ctx_t *ctx)
{

	char ch = lexer_get_char(ctx);

	if (ch == '\0')
	{
		return 0;
	}
	else if (ch == 10)
	{
		ctx->locale.row++;
		ctx->locale.col = 0;
		ctx->pos++;
		return 0;
	}
	ctx->locale.col++;
	ctx->pos++;
	return 1;
}

#define ADVANCE lexer_advance(ctx)

char *lexer_slice(lexer_ctx_t *ctx, size_t start, size_t stop)
{
	//printf("start: %d stop: %d\n", start, stop);
	if (start > stop)
		return to_string(0);
	if (stop > ctx->src_len)
	{
		return to_string(0);
	}

	char *string = CALLOC(stop - start + 1, char);
	memcpy(string , ctx->src + start, stop - start);
	return string;
}

#define SLICE(start, stop) lexer_slice(ctx, start, stop)

void print_token(tok_t *tok)
{
	if(!target.debug_enabled)
		return;
	printf("lexme: %s type: %d  row:%d col:%d fid:%d\n", tok->lexeme, tok->type, tok->locale.row, tok->locale.col, tok->locale.file);
}

lexer_ctx_t *create_token_stream(char *src, size_t file_id)
{
	const int default_alloc = 100;
	tok_t *tokens = CALLOC(default_alloc, tok_t);

	lexer_ctx_t *ctx = CALLOC(1, lexer_ctx_t);
	toklocale_t local = {.file = file_id, .col = 0, .row = 1};
	ctx->locale = local;
	ctx->pos = 0;
	ctx->capacity = default_alloc;
	ctx->count = 0;
	ctx->src = src;
	ctx->src_len = strlen(src);
	ctx->toks = tokens;

	char ch = 1;
	size_t start = 0;
	int index = 0;
	bool seperator = false;
	while (ch != 0)
	{
		ch = GETCHAR;
		++index;
		//printf("%c\n", ch);
		if(ch == 0)
		{
			break;
		}
		if (isspace(ch))
		{
			//printf("0, index: %d %c\n", ch, ch);

			if (ch == '\n')
				EMIT(TOK_NEWLINE, to_string('\\'));
			else if(seperator == false)
			{
				seperator = true;
				EMIT(TOK_IDENT, to_string(' '));
			}
			ADVANCE;
			continue;
		}

		else if (isalpha(ch) || ch == '_')
		{
			//printf("1, index: %d %d\n", index, ctx->pos);

			start = ctx->pos;
			while (isalnum(PEEK) || PEEK == '_')
			{
				//printf("peek: %d %c\n", PEEK, PEEK);
				ADVANCE;
			}
			ADVANCE;

			char *lexeme = SLICE(start, ctx->pos);
			//printf("lexme: %s\n", lexeme);


			if (GETCHAR == ':')
			{
				EMIT(TOK_REFERENCE, lexeme);
			}
			else
			{
				EMIT(TOK_TOKEN, lexeme);
			}
			seperator = false;
			continue;
		}

		else if (isdigit(ch) || ch == '-')
		{
			//printf("2, index: %d %d\n", index, ctx->pos);


			start = ctx->pos;
			bool onef = false;
			while (isxdigit(PEEK) || PEEK == 'x')
			{
				//printf("peek: %d %c\n", PEEK, PEEK);

				ADVANCE;
			}
			ADVANCE;

			EMIT(TOK_NUMBER, SLICE(start, ctx->pos));
			seperator = false;
			continue;
		}

		else if (ch == ';')
		{
			//printf("3\n");
			//printf("3, index: %d %d\n", index, ctx->pos);

			start = ctx->pos;
			while ( PEEK != '\n' && PEEK != EOF )
				ADVANCE;
			ADVANCE;
			EMIT(TOK_COMMENT, SLICE(start, ctx->pos) );
			seperator = false;
			continue;
		}

		else if (ch == '"')
		{
			//printf("4, index: %d %d\n", index, ctx->pos);

			start = ctx->pos;
			while (PEEK != '"' && PEEK != EOF)
				ADVANCE;
			ADVANCE;
			ADVANCE; // consume closing "
			EMIT(TOK_STRING, SLICE(start, ctx->pos));
			seperator = false;
			continue;
		}

		// symbols
		else if (strchr("[]()+{}", ch))
		{
			//printf("5, index: %d %c\n",ch, ch);

			EMIT(TOK_SYMBOL, to_string(ch));
			ADVANCE;

			continue;
		}

		else if(ch == ':')
		{
			EMIT(TOK_COLON, to_string(ch));
			seperator = false;
			ADVANCE;
		}
		else if(ch == ',')
		{
			EMIT(TOK_COMMA, to_string(ch));
			seperator = true;
			ADVANCE;
		}
		else if(ch == '.')
		{
			//printf("%d %c\n", index, ch);

			if(isalpha(PEEK) && ctx->locale.col == 0)
			{
				start = ctx->pos;

				while(isalnum(PEEK) || PEEK == '_')
				{
					//printf("%d %c\n", index, ch);

					ADVANCE;
				}
				ADVANCE;

				char *lexeme = SLICE(start, ctx->pos);

				EMIT(TOK_DIRECTIVE, lexeme);

			}
			else
			{
				EMIT(TOK_DOT, to_string(ch));
				ADVANCE;

			}
			seperator = false;
		}
		else if(ch == '@')
		{

			if(isalpha(PEEK))
			{
				start = ctx->pos;

				while(isalnum(PEEK) || PEEK == '_')
				{
					//printf("%d %c\n", index, ch);

					ADVANCE;
				}
				ADVANCE;

				char *lexeme = SLICE(start, ctx->pos);

				EMIT(TOK_TOKEN, lexeme);

			}
			seperator = false;

		}
		else if(ch == '%')
		{

			if(isalpha(PEEK))
			{
				start = ctx->pos;

				while(isalnum(PEEK) || PEEK == '_')
				{
					//printf("%d %c\n", index, ch);

					ADVANCE;
				}
				ADVANCE;

				char *lexeme = SLICE(start, ctx->pos);

				EMIT(TOK_PERCENT, lexeme);

			}
			seperator = false;

		}
		else if(ch == '#')
		{
			if(isdigit(PEEK))
			{
				start = ctx->pos;

				while(isdigit(PEEK))
				{
					//printf("%d %c\n", index, ch);

					ADVANCE;
				}
				ADVANCE;

				char *lexeme = SLICE(start, ctx->pos);

				EMIT(TOK_TOKEN, lexeme);

			}
			seperator = false;
		}
		else
		{

			perror("Unknown character");
			ADVANCE;
			seperator = false;
		}


	}
	EMIT(TOK_EOF, to_string(0));

	for(int i = 0; i < ctx->count; ++i)
	{
		//print_token(&ctx->toks[i]);
	}
	return ctx;
}