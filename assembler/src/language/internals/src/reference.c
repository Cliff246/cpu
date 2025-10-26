#include "reference.h"
#include "commons.h"
#include "stdio.h"

void free_ref(void *ptr)
{
	ref_t *ref = (ref_t *)ptr;
	free(ref->ref_string);
	free(ref);
}

ref_t *create_reference( char *key)
{
	ref_t *ref = CALLOC(1, ref_t);

	ref->resolved = false;
	ref->implemented = false;
	ref->resolved_address = 0;

	ref->ref_string = strdup(key);
	return ref;
}

void implement_reference(ref_t *ref, parse_node_t *head)
{
	ref->row = head->tok->locale.row;
	ref->col = head->tok->locale.col;
	ref->fid = head->tok->locale.file;

	ref->head = head;
	ref->resolved = false;
	ref->implemented = true;
	ref->locale_offset = 0;
	ref->resolved_address = 0;


}