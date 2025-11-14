#include "reference.h"
#include "eerror.h"
#include "commons.h"
#include "fileio.h"
#include <stdio.h>

void free_reference(ref_t *ptr)
{
	ref_t *ref = (ref_t *)ptr;
	free(ref);
}


void create_ref_error(tok_t *tok, char *error)
{

	errelm_line_t line = {.column = get_token_col(tok), .line = get_token_row(tok)};
	errelm_file_t file = {.name = get_filename_from_id(get_token_file(tok))};
	char buffer[1025] = {0};
	errelm_t elmline = errelm_create_line_element(line);
	errelm_t elmfile =  errelm_create_file_element(file);
	//printf("emit stage\n");
	sprintf(buffer, "reference %s is not a valid reference name", error);

	//printf("emit stage\n");
	emit_error(TOKEN_ERROR, buffer, 2, elmfile, elmline);
}

void print_ref(ref_t *ref)
{
	printf("%15s b:%d l:%d f:%d a:%d r:%d\n", ref->ref_string, ref->byte_offset, ref->locale_offset, ref->fragment_offset, ref->absolute_offset, ref->resolved_address);
}

ref_t *create_reference( char *key)
{
	ref_t *ref = CALLOC(1, ref_t);


	if(valid_name(key))
	{
		if(strcmp(key, "acc"))
		{
			strncpy(ref->ref_string, key, REFERENCE_STRING_SIZE);
			ref->resolved = false;
			ref->implemented = false;
			ref->resolved_address = 0;
			ref->has_error = false;

			return ref;
		}



	}
	ref->resolved = false;
	ref->implemented = false;
	ref->resolved_address = 0;
	ref->has_error = true;
	return ref;




}

parse_node_t *get_arguments_under_reference(ref_t *ref)
{
	parse_node_t *args = ref->head->children[0];
	return args;
}

parse_node_t *get_entries_under_reference(ref_t *ref)
{
	parse_node_t *entries = ref->head->children[1];
	return entries;
}

void implement_reference(ref_t *ref, parse_node_t *head)
{
	if(head->child_count != 2)
	{
		printf("references should implement two children implmeneted %s\n", head->child_count);
		escape(1);
	}

	if(ref->has_error)
	{
		create_ref_error(head->tok, head->tok->lexeme);

	}

	ref->row = head->tok->locale.row;
	ref->col = head->tok->locale.col;
	ref->fid = head->tok->locale.file;

	ref->head = head;
	ref->resolved = false;
	ref->implemented = true;
	ref->locale_offset = 0;
	ref->resolved_address = 0;


}

