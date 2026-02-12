#ifndef __CFG_COMMONS_HEADER__
#define __CFG_COMMONS_HEADER__

#include <stdint.h>

typedef int64_t CFG_edge_id_t;
typedef int64_t CFG_node_id_t;
typedef int64_t CFG_link_id_t;

void CFG_free_tag_hashtable(void *v);

#endif