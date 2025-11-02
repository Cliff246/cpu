#include "reference.h"
#include "commons.h"
#include "stdio.h"

void free_reference(ref_t *ptr)
{
	ref_t *ref = (ref_t *)ptr;
	free(ref);
}

void print_ref(ref_t *ref)
{
	printf("%s %d:%d\n", ref->ref_string, ref->locale_offset, ref->resolved_address);
}

ref_t *create_reference( char *key)
{
	ref_t *ref = CALLOC(1, ref_t);

	strncpy(ref->ref_string, key, REFERENCE_STRING_SIZE);


	ref->resolved = false;
	ref->implemented = false;
	ref->resolved_address = 0;

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
		exit(1);
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

