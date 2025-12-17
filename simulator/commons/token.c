#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "commons.h"

tok_t empty_tok = {.token = "/", .type = TOK_NONE};
//string should be it a valid ptr to take, this is a move
static void emit(toklex_t *tl, tok_type_t type, char *string)
{

	tl->tokens = realloc_safe(tl->tokens, tl->tcount + 1, sizeof(tok_t));

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


static char peek(toklex_t *tl)
{
	if(tl->index >= tl->slen)
		return 0;
	else
		return tl->string[tl->index];

}

static char advance(toklex_t *tl)
{

 	char pk = peek(tl);
	tl->index++;
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
	tl->tokens = calloc(1, sizeof(tok_t));
	tl->tcount = 0;
	tl->slen = len;

	int seperators = 0;
	while(tl->index < tl->slen)
	{

		size_t begin = position(tl);
		char cur = advance(tl);
		if(isblank(cur))
		{
			if(cur == 10)
			{
			}
		}
		else if(isalpha(cur))
		{

			do
			{
				char scroll = peek(tl);
				if(!isalnum(scroll) && scroll != '_')
				{
					break;
				}
			}
			while(advance(tl));
			size_t end = position(tl);

			//printf("done scroll\n");

			char *spl = split(tl, begin, end);

			emit(tl, TOK_WORD, spl);


		}
		else if(cur == '\"')
		{
			do
			{
				char scroll = peek(tl);
				if(scroll == 0 || scroll == 10)
					break;
				if(scroll == '\"')
					break;
			}
			while(advance(tl));
			size_t end = position(tl);
			if(end == begin + 1)
			{
				emit(tl, TOK_STRING, strdup("\"\""));
				advance(tl);
			}
			else
			{
				char *spl = split(tl, begin + 1, end);
				emit(tl, TOK_STRING, spl);

				advance(tl);
			}
		}
		else if(isdigit(cur))
		{
			//printf("digit\n");

			if(cur == '0')
			{
				char next = advance(tl);
				if(next == 'x' || next == 'X')
				{

					do {
						char scroll = peek(tl);
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
				if(next == 'b' || next == 'B')
				{
					do
					{
						char scroll = peek(tl);
						if(scroll != '0' && scroll != '1')
						{
							break;
						}
					}
					while(advance(tl));
					size_t end = position(tl);

					emit(tl, TOK_BIN, split(tl, begin, end - 1));

				}
				else if(next < '8' && next >= '0')
				{
					do 					{
						char scroll = peek(tl);
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
				do
				{
					if(!isdigit(peek(tl)))
					{
						break;
					}
				}
				while(advance(tl));
				size_t end = position(tl);
				emit(tl, TOK_INT, split(tl, begin, end));
			}

		}
		else if(cur == '-')
		{

			char next = peek(tl);
			if(isdigit(next))
			{
				//negative number
				do
				{
					if(!isdigit(peek(tl)))
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
					char scroll = peek(tl);
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
			char next = peek(tl);
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
		else if(strchr("()[]{}", cur))
		{
			emit(tl, TOK_BRACKET, to_string(cur));
		}
		else if(cur == ',')
		{

			emit(tl, TOK_COMMA, to_string(cur));
		}
		else if(cur == '.')
		{

			emit(tl, TOK_DOT, to_string(cur));
		}

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

void free_tokstring(tok_t *token)
{
	free(token->token);

}
void free_toklex(toklex_t *toklex)
{
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

	if(index >= tl->tcount || index < 0)
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


tok_t *peek_toklex(toklex_t *tl)
{
	if(tl->index >= tl->tcount)
	{
		return NULL;
	}
	return &tl->tokens[tl->index++];
}

bool expect_toklex(toklex_t *tl, tok_type_t type)
{
	tok_t *tok = peek_toklex(tl);
	if(tok == NULL)
		return false;
	else
	{
		return (tok->type == type)? true : false;
	}
}