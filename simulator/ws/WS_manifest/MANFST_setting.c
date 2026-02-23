#include "MANFST_setting.h"
#include "SYNTAX_parser.h"
#include "token.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>



MANFST_setting_t *MANFST_init_setting(SYNTAX_pnode_t *setting)
{

	MANFST_setting_t *ptr = calloc(1, sizeof(MANFST_setting_t));

	assert(setting->size == 2);
	assert(setting->type == SYNTAX_PNODE_KEYWORD);
	assert(setting->token->type == TOK_INT);
	char *id_str = setting->token->token;
	ptr->id = atoi(id_str);

	SYNTAX_pnode_t *latency = setting->nodes[0];

	assert(latency->type == SYNTAX_PNODE_VALUE);
	assert(latency->token->type == TOK_INT);

	char *latency_str = latency->token->token;
	ptr->latency = atoi(latency_str);

	SYNTAX_pnode_t *throughput = setting->nodes[1];


	assert(throughput->type == SYNTAX_PNODE_VALUE);
	assert(throughput->token->type == TOK_INT);
	char *throughput_str = throughput->token->token;
	ptr->throughput = atoi(throughput_str);
	return ptr;
}

void MANFST_free_setting(MANFST_setting_t *setting)
{
	free(setting);

}

