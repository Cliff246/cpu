#ifndef __SEGMENT_HEADER__
#define __SEGMENT_HEADER__

#include "parser.h"
#include <stdint.h>
#include "entry.h"
#include <stdbool.h>
#include "hashmap.h"

#define MAX_SEGID 1000
#define MAX_TAGS 100
extern int segment_ids_to_tag[MAX_SEGID];

#define MAX_TAG_SIZE 10

typedef struct tag_store
{
	char key[MAX_TAG_SIZE + 1];
	int tag;
}tag_store_t;

#define SEGMENT_TAGS_TABLE_SIZE MAX_TAGS



tag_store_t *new_tag_store(char *key);

void free_tag_store(void *ptr);
void init_tag_store(void);

int check_tag(char *key);
int register_tag(char *key);

extern p_hashtable_t segment_tags;


typedef enum segment_type
{
	ISEG_NONE,
	ISEG_REALLOC,		//realloc segments
	ISEG_TEXT,			//code semgents that are seperate
	ISEG_DATA,			//data segments
	ISEG_BSS,			//bss
	ISEG_CODE,			//combined unified code blocks
}seg_type_t;

#define SEGMENT_ARUGMENT_STRING_SIZE 100

typedef struct segment_argument
{
	char arg[SEGMENT_ARUGMENT_STRING_SIZE + 1];
	bool used;
}seg_arg_t;


#define SEGMENT_MAX_ARGUMENTS 5

typedef struct segment
{
	seg_type_t segtype;
	int sid;

	seg_arg_t args[SEGMENT_MAX_ARGUMENTS];
	bool state_refat;
	//todo do ref at
	bool state_tag;
	int tag;

}seg_t;

seg_t create_segment(parse_node_t *head);


char *get_seg_type_string(seg_type_t segtype);

#endif