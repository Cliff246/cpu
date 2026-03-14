#include "token.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "commons.h"

tok_t empty_tok = {.token = "/", .type = TOK_NONE};
//string should be it a valid ptr to take, this is a move
static void emit(toklex_t *tl, tok_type_t type, char *string)
{
	if(tl->tcount == tl->allocd)
	{
		//printf("realloc %d\n",tl->allocd );

		tl->allocd = (tl->allocd * 2) + 1;
		tl->tokens = realloc_safe(tl->tokens, tl->allocd, sizeof(tok_t));
	}
	tl->tokens[tl->tcount].token = string;
	tl->tokens[tl->tcount].type = type;
	tl->tcount++;
}


static char *split(toklex_t *tl, size_t start, size_t stop)
{

	if(stop < start)
	{
		printf("null\n");
		return NULL;
	}
	if(tl->slen < stop)
	{
		printf("null\n");
		return NULL;
	}
	if(stop == start)
	{
		return to_string(tl->string[start]);
	}
	char *split =  calloc(stop - start + 1, sizeof(char));
	memcpy(split, tl->string + start, stop - start);



	return split;
}


static char peak(toklex_t *tl)
{
	if(tl->index >= tl->slen)
		return 0;
	else
		return tl->string[tl->index];

}

static char next(toklex_t *tl)
{
	if(tl->index + 1 >= tl->slen)
	{
		return -1;
	}
	else
		return tl->string[tl->index + 1];
}

static char advance(toklex_t *tl)
{

	tl->index++;
 	char pk = peak(tl);

	return pk;
}

static size_t position(toklex_t *tl)
{
	return tl->index;
}




toklex_t *lex_string(const char *string)
{

	toklex_t *tl = calloc(1, sizeof(toklex_t));
	if(tl == NULL)
		//scuffed
		return NULL;

	char *dup = strdup(string);
	size_t len = strlen(dup);

	tl->string = dup;
	tl->index = 0;
	tl->allocd = 10;
	tl->tokens = calloc(tl->allocd, sizeof(tok_t));
	tl->tcount = 0;
	tl->slen = len;

	int seperators = 0;
	while(tl->index < tl->slen)
	{

		size_t begin = position(tl);
		char cur = peak(tl);
		//printf("\'%c\'\n", cur);
		if(cur == '\n')
		{
			emit(tl, TOK_NEWLINE, to_string('~'));

		}
		else if(isblank(cur))
		{

		}
		else if(isalpha(cur))
		{

			while(true)
			{
				char scroll = next(tl);
				if(!isalnum(scroll) && scroll != '_')
				{
					break;
				}
				advance(tl);
			}

			size_t end = position(tl) + 1;


			char *spl = split(tl, begin, end);

			emit(tl, TOK_WORD, spl);


		}
		else if(cur == '\"')
		{
			while(advance(tl))
			{

				char scroll = peak(tl);
				if(scroll == 0 || scroll == 10)
					break;
				if(scroll == '\"')
					break;
			}

			size_t end = position(tl);
			if(end == begin + 1)
			{
				emit(tl, TOK_STRING, strdup("\"\""));
			}
			else
			{
				char *spl = split(tl, begin + 1, end);
				emit(tl, TOK_STRING, spl);

			}
		}
		else if(isdigit(cur))
		{
			//printf("digit\n");

			if(cur == '0')
			{
				char after = next(tl);
				//printf("after %c %c\n",cur, after);
				if(after == 'x' || after == 'X')
				{
					assert(0 && "todo");

					do {
						char scroll = peak(tl);
						if(!isdigit(scroll) && !(scroll > 'A' && scroll < 'F') && !(scroll > 'a' && scroll < 'f'))
						{
							break;
						}
					}
					while(advance(tl));

					size_t end = position(tl);

					emit(tl, TOK_HEX, split(tl, begin, end ));

					//deal with hex
				}
				if(after == 'b' || after == 'B')
				{
					assert(0 && "todo");

					do
					{
						char scroll = peak(tl);
						if(scroll != '0' && scroll != '1')
						{
							break;
						}
					}
					while(advance(tl));
					size_t end = position(tl);

					emit(tl, TOK_BIN, split(tl, begin, end - 1));

				}
				else if(after < '8' && after > '0')
				{
					assert(0 && "todo");

					do {
						char scroll = peak(tl);
						if(scroll > '8' && scroll < '0')
						{
							break;
						}
					}
					while(advance(tl));

					size_t end = position(tl);

					emit(tl, TOK_OCT, split(tl, begin, end));

					//deal with octal
				}
				else
				{

					size_t end = position(tl);
					emit(tl, TOK_INT, split(tl, begin, end));
				}

			}
			else
			{
				while(true)
				{
					if(!isdigit(next(tl)))
						break;
					advance(tl);
				}
				size_t end = position(tl)+1;
				emit(tl, TOK_INT, split(tl, begin, end));
			}

		}
		else if(cur == '-')
		{
			assert(0 && "todo");

			char next = peak(tl);
			if(isdigit(next))
			{
				//negative number
				do
				{
					if(!isdigit(peak(tl)))
					{
						break;
					}
				}
				while(advance(tl));
				size_t end = position(tl);
				emit(tl, TOK_INT, split(tl, begin, end));

			}
			else if(next == '=')
			{

				advance(tl);
				size_t end = position(tl);
				emit(tl, TOK_OP, split(tl, begin, end));
			}
			else if(next == '-')
			{

				advance(tl);
				size_t end = position(tl);
				emit(tl, TOK_OP, split(tl, begin, end));

			}
			else
			{
				do
				{
					char scroll = peak(tl);
					if(!isalnum(scroll) && scroll != '_')
					{
						break;
					}
				}
				while(advance(tl));
				size_t end = position(tl);


				char *spl = split(tl, begin, end);
				emit(tl, TOK_WORD, spl);

			}


		}
		else if(strchr("=<>+*%/!@$|%^&", cur))
		{
			//printf("neg number\n");

			char next = peak(tl);
			if(next == cur)
			{

				advance(tl);
				size_t end = position(tl);
				if(next == '+')
				{
					emit(tl, TOK_OP, split(tl, begin, end));
				}
				else if(next == '=')
				{

					emit(tl, TOK_OP, split(tl, begin, end));
				}
				else if(next == '<')
				{

					emit(tl, TOK_OP, split(tl, begin, end));
				}
				else if(next == '>')
				{
					emit(tl, TOK_OP, split(tl, begin, end));
				}
			}
			else
			{
				emit(tl, TOK_OP, to_string(cur));
			}

		}
		else if(strchr("[]", cur))
		{
			emit(tl, TOK_SQUARE, to_string(cur));

		}
		else if(strchr("()", cur))
		{
			emit(tl, TOK_BRACKET, to_string(cur));
		}
		else if(strchr("{}", cur))
		{
			emit(tl, TOK_CURL, to_string(cur));
		}
		else if(cur == ',')
		{

			emit(tl, TOK_COMMA, to_string(cur));
		}
		else if(cur == '.')
		{

			emit(tl, TOK_DOT, to_string(cur));
		}
		else if(cur == ':')
		{
			emit(tl, TOK_COLON, to_string(cur));
		}
		else if(cur == ';')
		{
			emit(tl, TOK_SEMICOLON, to_string(cur));
		}
		advance(tl);
	}
	emit(tl, TOK_END, to_string('%'));
	tl->index = 0;
	return tl;
}

void print_tok(tok_t *token)
{

	printf("\"%s\" %d\n", token->token, token->type);
}

void print_toklex(toklex_t *tl)
{
	for(int i = 0; i < tl->tcount; ++i)
	{
		print_tok(&tl->tokens[i]);
	}
}

//dont use this on toklex, only on copies
void free_tok(tok_t *tok)
{
	free_tokstring(tok);
	free(tok);
}


void free_tokstring(tok_t *token)
{
	//printf("free tok\n");
	free(token->token);

}
void free_toklex(toklex_t *toklex)
{
	//printf("free toklex\n");

	for(int i = 0; i < toklex->tcount; ++i)
	{
		free_tokstring(&toklex->tokens[i]);

	}
	free(toklex->tokens);
	free(toklex->string);
	free(toklex);
}

void reset_toklex(toklex_t *tl)
{
	tl->index = 0;
}

tok_t *get_toklex(toklex_t *tl, int index)
{

	if(index > tl->tcount || index < 0)
	{
		return NULL;
	}
	else
	{
		tok_t *tok = &tl->tokens[index];
		//printf("%s\n", tok->token);
		return tok;
	}
}


tok_t *peak_toklex(toklex_t *tl)
{
	if(tl->index > tl->tcount)
	{
		return NULL;
	}
	return &tl->tokens[tl->index++];
}

bool expect_toklex(toklex_t *tl, tok_type_t type)
{
	tok_t *tok = peak_toklex(tl);
	if(tok == NULL)
		return false;
	else
	{
		return (tok->type == type)? true : false;
	}
}

tok_t *copy_tok(tok_t *tok)
{
	tok_t *copy = calloc(1, sizeof(tok_t));
	copy->token = strdup(tok->token);
	copy->type = tok->type;
	return copy;
}