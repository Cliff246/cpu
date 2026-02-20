#include "CFG_setting.h"
#include "IO_parser.h"
#include "token.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

//super duper rough draft
CFG_setting_t *CFG_init_setting(IO_pnode_t *setting)
{

	assert(setting);
	CFG_setting_t *ptr = calloc(1, sizeof(CFG_setting_t));

	assert(setting->size == 2);
	assert(setting->type == IO_PNODE_KEYWORD);
	assert(setting->token->type == TOK_INT);
	char *id_str = setting->token->token;
	ptr->id = atoi(id_str);

	IO_pnode_t *latency = setting->nodes[0];

	assert(latency->type == IO_PNODE_VALUE);
	assert(latency->token->type == TOK_INT);

	char *latency_str = latency->token->token;
	ptr->latency = atoi(latency_str);

	IO_pnode_t *throughput = setting->nodes[1];


	assert(throughput->type == IO_PNODE_VALUE);
	assert(throughput->token->type == TOK_INT);
	char *throughput_str = throughput->token->token;
	ptr->throughput = atoi(throughput_str);
	return ptr;
}

void CFG_free_setting(CFG_setting_t *setting)
{
	free(setting);
}