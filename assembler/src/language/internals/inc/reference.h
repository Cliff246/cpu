#ifndef __REFERENCE_HEADER__
#define __REFERENCE_HEADER__

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "parser.h"
#include "entry.h"

typedef enum access_modifier
{
	ACCESS_INVAL,
	ACCESS_PUBLIC,
	ACESSS_PRIVATE,


}refmod_t;

#define REFERENCE_STRING_SIZE 100

typedef struct reference
{


	char ref_string[REFERENCE_STRING_SIZE + 1];
	//file id
	int fid;
	//segment id
	int sid;
	//row/col
	int row, col;


	//the locale/segment offset
	uint64_t locale_offset;
	uint64_t fragment_offset;
	uint64_t absolute_offset;

	uint64_t resolved_address;

	//public or
	refmod_t access_modifier;
	parse_node_t *head;
	//

	//
	bool resolved;
	bool implemented;
}ref_t;

parse_node_t *get_arguments_under_reference(ref_t *ref);

parse_node_t *get_entries_under_reference(ref_t *ref);

ref_t *create_reference(char *key);
void print_ref(ref_t *ref);
void implement_reference(ref_t *ref, parse_node_t *head);
void free_reference(ref_t *ptr);
#endif