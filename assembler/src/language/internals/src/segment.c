#include "segment.h"
#include <string.h>
#include "commons.h"




seg_type_t get_seg_type(char *node)
{

	const char *segment_types[] = {
		".realloc",
		".text",
		".data",
		".bss",
		".code",
		".config",
	};
	seg_type_t type = 0;
	for(int i = 0; i < ARYSIZE(segment_types); ++i)
	{
		if(!strcmp(node, segment_types[i]))
		{
			type = i + 1;
		}
	}
	return type;
}




seg_t create_segment(parse_node_t *head)
{
	static int segid = 0;
	seg_t segment = {0};

	segment.sid = segid++;


	//find type
	//TODO
	//segment->segtype =
	//
	//
	//
	segment.segtype = get_seg_type(head->tok->lexeme);

	LOG("seg type %d\n", segment.segtype, 0);
	return segment;
}

