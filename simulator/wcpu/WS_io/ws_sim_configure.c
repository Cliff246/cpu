#include "ws_sim_configure.h"
#include "loader.h"
#include "module.h"
#include "token.h"
#include "device_command_impl.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

WS_cfg_elem_t WS_cfg_create_element(tok_t **buf, size_t size)
{

	WS_cfg_elem_t entry = {0};

	if(size < 2)
	{
		fprintf(stderr,"error at element");
		exit(EXIT_FAILURE);
	}

	tok_t *key = buf[0];
	tok_t *value = buf[1];




	entry.key = copy_tok(key);
	entry.value = copy_tok(value);

	return entry;
}

WS_cfg_body_t *WS_cfg_create_body(void)
{
	WS_cfg_body_t *body =  calloc(1, sizeof(WS_cfg_body_t));
	return body;
}


void WS_cfg_append_body(WS_cfg_body_t *body, WS_cfg_elem_t elem)
{
	assert(body);
	body->elements = realloc_safe(body->elements, body->size + 1, sizeof(WS_cfg_elem_t));

	body->elements[body->size++] = elem;

}

void WS_cfg_append_header(WS_cfg_hdr_t *header, WS_cfg_body_t *body)
{
	assert(body);
	header->bodies = realloc_safe(header->bodies, header->size + 1, sizeof(WS_cfg_body_t*));

	header->bodies[header->size++] = body;
}

void WS_cfg_print_element(WS_cfg_elem_t *elem)
{
	printf("{[key] = %s} {[value] = %s}\n", elem->key->token, elem->value->token);
}

void WS_cfg_print_body(WS_cfg_body_t *body)
{
	for(int i = 0; i < body->size; ++i)
	{
		WS_cfg_print_element(&body->elements[i]);
	}
}

void WS_cfg_collect_body(WS_cfg_body_t *body)
{
	WS_dev_cmd_collection_t *collect = WS_cmd_collection_create();

	//printf("body size: %d\n", body->size);

	for(int i = 0; i < body->size; ++i)
	{
		WS_cfg_elem_t elem = body->elements[i];
		//printf("%d\n", i);
		WS_dev_flag_t *flag =  WS_dev_flag_create(elem.key, elem.value);
		WS_cmd_collection_append(collect, flag);
	}

	body->collection = collect;

}

void WS_cfg_collect_header(WS_cfg_hdr_t *header)
{
	header->cmds = calloc(header->size, sizeof(WS_dev_cmd_t *));
	assert(header->cmds);
	for(int i = 0; i < header->size; ++i)
	{
		header->cmds[i] = WS_dev_cmd_create(header->bodies[i]->collection, header->module->dev_desc);
	}
}


static void load_cfg_file(WS_cfg_file_t *file)
{
	if(!open_sourcefile(file->srcfile))
	{
		fprintf(stderr, "could not open srcfile %s", file->srcfile->path);
		exit(EXIT_FAILURE);
	}






	int length = get_sourcefile_length(file->srcfile);
	if(length <= 0)
	{
		fprintf(stderr, "could not get length of srcfile %s:%d\n", file->srcfile->path, length);
		exit(EXIT_FAILURE);
	}


	char *fbuf = read_sourcefile(file->srcfile, length);
	//printf("%s\n", buffer);
	toklex_t *tl = lex_string(fbuf);
	reset_toklex(tl);
	size_t tbuf_max_size = 100;
	tok_t **tbuf = calloc(tbuf_max_size, sizeof(tok_t *));
	size_t tbuf_used = 0;

	WS_cfg_hdr_t *cur_head = NULL;
	WS_cfg_body_t *cur_body = NULL;

	enum
	{
		HEAD_STATE_EMPTY,
		HEAD_STATE_STRING,
		HEAD_STATE_COLON,
		HEAD_STATE_BODY,
	}cur_head_state = HEAD_STATE_EMPTY;


	//print_toklex(tl);

	while(true)
	{



		tok_t *tok = peek_toklex(tl);
		//printf("%d %s\n",cur_head_state, tok->token);
		//printf("%p %p\n", cur_head, cur_body);
		if(tok->type == TOK_END)
		{
			break;
		}
		else if(tok->type == TOK_NEWLINE)
		{
			continue;
		}
		else if(tok->type == TOK_SEMICOLON)
		{
			cur_head_state = HEAD_STATE_EMPTY;
			WS_cfg_collect_header(cur_head);
			WS_cfg_append_file(file, cur_head);
			cur_head = NULL;
		}


		else if(tok->type == TOK_STRING && cur_head_state == HEAD_STATE_EMPTY)
		{
			WS_module_t *module = WS_module_create(tok->token);
			cur_head = WS_cfg_create_header(module);
			cur_head_state = HEAD_STATE_STRING;
		}
		else if(tok->type == TOK_COLON && cur_head_state == HEAD_STATE_STRING)
		{
			cur_head_state = HEAD_STATE_COLON;
		}

		else if(tok->type == TOK_BRACKET && !strcmp(tok->token, "{") && cur_head_state == HEAD_STATE_COLON)
		{
			cur_head_state = HEAD_STATE_BODY;
			cur_body = WS_cfg_create_body();

		}
		else if(tok->type == TOK_BRACKET && !strcmp(tok->token, "}")  && cur_head_state == HEAD_STATE_BODY )
		{

			WS_cfg_collect_body(cur_body);
			WS_cfg_append_header(cur_head, cur_body);
			cur_body = NULL;

			memset(tbuf, 0, tbuf_max_size * sizeof(tok_t *));
			tbuf_used = 0;
			cur_head_state = HEAD_STATE_COLON;
		}
		else if(cur_head_state == HEAD_STATE_BODY)
		{

			//has current head
			if(tok->type == TOK_DOT)
			{
				WS_cfg_elem_t elem = WS_cfg_create_element(tbuf, tbuf_used);
				WS_cfg_append_body(cur_body, elem);
				//WS_cfg_print_element(&elem);

				memset(tbuf, 0, tbuf_max_size * sizeof(tok_t *));
				tbuf_used = 0;

			}
			else
			{
				if(tbuf_max_size <= tbuf_used)
				{
					tbuf = realloc_safe(tbuf, tbuf_max_size *= 2, sizeof(tok_t *));
				}
				tbuf[tbuf_used++] = tok;
				//printf("added to tbuf %d\n", tbuf_used);
			}
		}

		//print_toklex(toklex);
		//WS_config_module_container_t *container  =WS_create_config_module_container(line);
		//
	}



	//WS_append_config_file_module_container(file, container);
	free_toklex(tl);
	free(tbuf);
	free(fbuf);

	close_sourcefile(file->srcfile);

}


WS_cfg_hdr_t *WS_cfg_create_header(WS_module_t *module)
{
	WS_cfg_hdr_t *header =  calloc(1, sizeof(WS_cfg_hdr_t));


	header->module = module;

	return header;
}


//so unsafe lol
void WS_cfg_append_file(WS_cfg_file_t *file, WS_cfg_hdr_t *header)
{
	file->headers = realloc_safe(file->headers, file->size + 1, sizeof(WS_cfg_hdr_t*));
	file->headers[file->size++] = header;
}


WS_cfg_file_t *WS_cfg_create_file(const char *path)
{



	WS_cfg_file_t *config = calloc(1, sizeof(WS_cfg_file_t));

	sourcefile_t *sf = create_sourcefile((char *)path);

	assert(get_canopen_sourcefile(sf));

	config->srcfile = sf;


	load_cfg_file(config);









	return config;
}

void WS_cfg_free_body(WS_cfg_body_t *body)
{


	for(int i = 0; i < body->size; ++i)
	{
		free_tok(body->elements[i].key);
		free_tok(body->elements[i].value);
	}
	free(body->elements);
	free(body);
}

void WS_cfg_free_header(WS_cfg_hdr_t *header)
{

	for(int i = 0; i < header->size; ++i)
	{

		WS_cfg_free_body(header->bodies[i]);
	}

	free(header->bodies);
	free(header->cmds);
	free(header);
}




void WS_cfg_free_file(WS_cfg_file_t *file)
{

	for(int i = 0; i < file->size; ++i)
	{
		WS_cfg_free_header(file->headers[i]);
	}
	free(file->headers);
	free_sourcefile(file->srcfile);
	free(file);
}
