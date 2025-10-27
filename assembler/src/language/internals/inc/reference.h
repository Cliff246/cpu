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
	ACCESS_PUBLIC,
	ACESSS_PRIVATE,


}refmod_t;


typedef struct reference
{


	char *ref_string;
	//file id
	int fid;
	//segment id
	int sid;
	//row/col
	int row, col;


	//the locale/segment offset
	uint64_t locale_offset;
	uint64_t resolved_address;

	//public or
	refmod_t access_modifier;
	parse_node_t *head;
	//

	//
	bool resolved;
	bool implemented;
}ref_t;

ref_t *create_reference(char *key);
void print_ref(ref_t *ref);
void implement_reference(ref_t *ref, parse_node_t *head);
void free_reference(ref_t *ptr);
#endif