#include "wcpu_part.h"
#include "lsu.h"
#include "mattress.h"
#include "cache.h"
#include "aggregator.h"
#include "register_file.h"
#include "fetcher.h"
#include "mau.h"
#include "deployer.h"
#include "scheduler.h"

#include <assert.h>
#include <stdio.h>



wcpu_part_class_t part_vtable[UNIQUE_PARTS] =
{
	[WCPU_PART_LSU] =
	{
		.init = wcpu_lsu_generate,
		.step = wcpu_lsu_update,
		.export = wcpu_lsu_export,
		.import = wcpu_lsu_import
	},

	[WCPU_PART_MATTRESS] =
	{
		.init = wcpu_mattress_generate,
		.step = wcpu_mattress_update,
		.export = wcpu_mattress_export,
		.import = wcpu_mattress_import
	},

	[WCPU_PART_CACHE] =
	{
		.init = wcpu_cache_generate,
		.step = wcpu_cache_update,
		.export = wcpu_cache_export,
		.import = wcpu_cache_import
	},

	[WCPU_PART_AGGREGATOR] =
	{
		.init = wcpu_aggregator_generate,
		.step = wcpu_aggregator_update,
		.export = wcpu_aggregator_export,
		.import = wcpu_aggregator_import,
	},

	[WCPU_PART_FETCHER] =
	{
		.init = wcpu_fetcher_generate,
		.step = wcpu_fetcher_update,
		.export = wcpu_fetcher_export,
		.import = wcpu_fetcher_import
	},

	[WCPU_PART_REGFILE] =
	{
		.init = wcpu_regfile_generate,
		.step = wcpu_regfile_update,
		.export = wcpu_regfile_export,
		.import = wcpu_regfile_import
	},

	[WCPU_PART_LEDGER] =
	{
		.init = wcpu_ledger_generate,
		.step = wcpu_ledger_update,
		.export = wcpu_ledger_export,
		.import = wcpu_ledger_import
	},

	[WCPU_PART_MAU] =
	{
		.init = wcpu_mau_generate,
		.step = wcpu_mau_update,
		.export = wcpu_mau_export,
		.import = wcpu_mau_import
	},
	[WCPU_PART_DEPLOYER] =
	{
		.init = wcpu_deployer_generate,
		.step = wcpu_deployer_update,
		.export = wcpu_deployer_export,
		.import = wcpu_deployer_import,
	},
	[WCPU_PART_SCHEDULER] =
	{
		.init = wcpu_scheduler_generate,
		.step = wcpu_aggregator_update,
		.export = wcpu_aggregator_export,
		.import = wcpu_scheduler_import,
	},
	[WCPU_PART_CCU] =
	{

	}
};

#define PART_TYPE_STRING(X) [WCPU_PART_ ## X] = #X ,

char *part_type_strs[UNIQUE_PARTS] =
{
	WCPU_PART_LIST(PART_TYPE_STRING)
};


//should be a safe way to init a function
part_t *init_part(part_type_t type)
{
	static int part_id = 0;
	//printf("type: %d\n", type);
	assert(type >= 0 && type < UNIQUE_PARTS && "part outside of bounds");

	wcpu_part_class_t funcs = part_vtable[type];
	assert(funcs.init != NULL && "part must have an init");


	part_t *part = calloc(1, sizeof(part_t));
	assert(part != NULL && "part was not initalzied correctly");

	part->id = part_id++;
	part->type = type;
	void *impl_init = funcs.init().raw;
	assert(impl_init != NULL && "null *cannot be a part");
	part->ptr.raw = impl_init;
	return part;
}

//step part
void wcpu_part_step(part_t *part)
{

	assert(part != NULL && "part cannot be null when stepping");
	part_type_t type = part->type;
	//printf("%d\n", type);
	assert(type >= 0 && type < UNIQUE_PARTS && "part outside of bounds");

	wcpu_part_class_t funcs = part_vtable[type];
	assert(funcs.step != NULL && "part must not have a defined step");


	funcs.step(part);
	return;
}

part_t *wcpu_part_generate(part_type_t type)
{
	return init_part(type);
}


bool wcpu_part_import(part_t *part, part_signal_t *signal)
{
	assert(part != NULL && "part cannot be null when stepping");
	part_type_t type = part->type;
	assert(type >= 0 && type < UNIQUE_PARTS && "part outside of bounds");

	wcpu_part_class_t funcs = part_vtable[type];
	assert(funcs.import != NULL && "part must not have a defined import");
	bool result = funcs.import(part, signal);
	return result;
}

bool wcpu_part_export(part_t *part, part_signal_t **signal)
{
	assert(part != NULL && "part cannot be null when stepping");
	part_type_t type = part->type;
	assert(type >= 0 && type < UNIQUE_PARTS && "part outside of bounds");

	wcpu_part_class_t funcs = part_vtable[type];
	assert(funcs.export != NULL && "part must not have a defined export");
	bool result = funcs.export(part, signal);
	return result;
}