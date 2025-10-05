#ifndef __ANALYZER__
#define __ANALYZER__

#include <stdlib.h>


typedef struct analyzer_node
{
	struct analyzer_node **children;
	size_t child_len;
}analyzer_node_t;


#endif
