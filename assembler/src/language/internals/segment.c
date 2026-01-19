#include "segment.h"
#include "commons.h"
#include "eerror.h"
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int segment_ids_to_tag[MAX_SEGID] = {-1};
int tags_used = 0;


char *seg_type_builtins[] =
{
	[ISEG_NONE] = ".none",
	[ISEG_REALLOC] = ".realloc",
	[ISEG_TEXT] = ".text",
	[ISEG_DATA] = ".data",
	[ISEG_BSS] = ".bss",
	[ISEG_CODE] = ".code",
};

int allocate_tag(void)
{
	if(tags_used > MAX_TAGS)
	{
		printf("too many tags used%d\n", MAX_TAGS);
		escape(1);
	}
	return tags_used++;
}


void free_tag_store(void *ptr)
{
	tag_store_t *tptr = (tag_store_t *)ptr;
	free(tptr);
}

tag_store_t *new_tag_store(char *key)
{
	tag_store_t *tag =  CALLOC(1, tag_store_t);

	strncpy(tag->key, key, MAX_TAG_SIZE);
	tag->tag = allocate_tag();
	return tag;
}

p_hashtable_t segment_tags;



void init_tag_store(void)
{
	static bool has_init = false;
	if(has_init == true)
		return;
	has_init = true;
	segment_tags = new_hash_table(SEGMENT_TAGS_TABLE_SIZE, free_tag_store);
}

int check_tag(char *key)
{
	print_hash_table(segment_tags);
	if(key == NULL)
	{
		//fprintf(stderr, "key failed");
		//exit(EXIT_FAILURE);
	}
	tag_store_t *tag =  (tag_store_t *)getdata_from_hash_table(segment_tags, key);
	if(tag == NULL)
		return -1;
	else
	{
		return tag->tag;
	}
}

int register_tag(char *key)
{
	//printf("reg tag %s\n", key);
	tag_store_t *tag =  new_tag_store(key);
	addto_hash_table(segment_tags, key, tag);
	return tag->tag;
}

static int assiociate_tag(char *key)
{
	int checked = check_tag(key);
	//printf("\n\nassiociate %s %d\n", key, checked);
	if(checked == -1)
	{
		return register_tag(key);
	}
	return checked;
}


seg_type_t get_seg_type(char *node)
{


	seg_type_t type = 0;
	for(int i = 1; i < ARYSIZE(seg_type_builtins) - 1; ++i)
	{

		if(!strcmp(node, seg_type_builtins[i]))
		{
			type = i;
		}
	}
	return type;
}

static void fill_segment_arguments(seg_t *seg, parse_node_t *head)
{

	size_t limit = MIN(SEGMENT_MAX_ARGUMENTS, head->child_count);

	for(int arg = 0; arg < limit; ++arg)
	{

		parse_node_t *child = head->children[arg];

		char *source_copy = child->tok->lexeme;

		memset(seg->args[arg].arg, 0, sizeof(char) * (SEGMENT_ARUGMENT_STRING_SIZE + 1));
		strncpy(seg->args[arg].arg, source_copy, SEGMENT_ARUGMENT_STRING_SIZE);
		seg->args[arg].used = true;
	}

}

static void generate_argument_data(seg_t *seg)
{

	bool tag_state = false;
	bool ref_state = false;
	bool at_state = false;


	for(int i = 0; i < SEGMENT_MAX_ARGUMENTS; ++i)
	{
		seg_arg_t *arg = &seg->args[i];
		//printf("tag %s\n", arg->arg);
		if(!arg->used)
			break;


		if(tag_state)
		{
			seg->state_tag = true;
			seg->tag = assiociate_tag(arg->arg);
			tag_state = false;
			arg->used = true;
		}
		else if(at_state)
		{

		}
		else if(ref_state)
		{
			seg->has_ref = i;
			arg->used = true;
			ref_state = false;
			//to fix a bug all references must be defined in start state
			seg->start_state = true;
		}

		// segment tag
		else if(!strcmp(arg->arg, "tag"))
		{
			//printf("tag %s\n", arg->arg);
			tag_state = true;
		}
		//segment
		else if(!strcmp(arg->arg, "at"))
		{
			at_state = true;
		}
		else if(!strcmp(arg->arg, "start"))
		{
			//LOG("Start state set\n", 0);
			seg->start_state = true;

		}
		else if(!strcmp(arg->arg, "ref"))
		{
			ref_state = true;
		}
		else
		{
			tag_state = false;
			at_state = false;
		}


	}

}


seg_t create_segment(parse_node_t *head)
{
	init_tag_store();
	static int segid = 0;
	seg_t segment = {0};
	segment.has_ref = -1;
	segment.sid = segid;

	if(segid >= MAX_SEGID)
	{
		printf("TOO many segments%d\n", segid);
		escape(1);
	}

	parse_node_t *arguments = (head->child_count <= 0)? NULL : head;


	if(arguments)
	{
		fill_segment_arguments(&segment, arguments);
		generate_argument_data(&segment);
	}
	else
	{
		segment.tag = allocate_tag();
	}

	//find type
	//TODO
	//segment->segtype =
	//
	//
	//
	segment.segtype = get_seg_type(head->tok->lexeme);
	segment.fid = head->tok->locale.file;
	//LOG("seg type %d\n", segment.segtype, 0);
	segment_ids_to_tag[segid++] = segment.tag;
	return segment;
}




char *get_seg_type_string(seg_type_t segtype)
{
	return seg_type_builtins[segtype];
}