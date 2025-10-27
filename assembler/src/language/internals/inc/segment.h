#ifndef __SEGMENT_HEADER__
#define __SEGMENT_HEADER__

#include "parser.h"
#include <stdint.h>
#include "entry.h"


typedef enum segment_type
{
	ISEG_NONE,
	ISEG_REALLOC,		//realloc segments
	ISEG_TEXT,			//code semgents that are seperate
	ISEG_DATA,			//data segments
	ISEG_BSS,			//bss
	ISEG_CODE,			//combined unified code blocks
}seg_type_t;




typedef struct segarg
{
	char *argument;


}segarg_t;


typedef struct segment
{
	seg_type_t segtype;
	int sid;


}seg_t;

seg_t create_segment(parse_node_t *head);

#endif