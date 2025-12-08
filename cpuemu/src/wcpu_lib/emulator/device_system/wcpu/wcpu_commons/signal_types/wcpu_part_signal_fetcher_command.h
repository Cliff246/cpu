#ifndef __WCPU_PART_SIGNAL_FETCHER_COMMAND_HEADER__
#define __WCPU_PART_SIGNAL_FETCHER_COMMAND_HEADER__

#include "wcpu_part_signal_ptr.h"

#include <stdint.h>
#include <stdbool.h>

#include "fetcher.h"


typedef enum
{
	FETCHER_COMMAND_CD_SWAP,
	FETCHER_COMMAND_CT_SET,
	FETCHER_COMMAND_CT_LOAD,
	FETCHER_COMMAND_INS_SET,
	FETCHER_COMMAND_INS_LOAD,
	FETCHER_COMMAND_IMM_SET,
	FETCHER_COMMAND_IMM_LOAD,
}_fetcher_command_type_t;

typedef struct
{
	uint64_t address;

}_fetcher_command_cd_swap_t;

typedef struct
{
	uint64_t cd_start;
	uint64_t cd_len;

}_fetcher_command_ct_set_t;

typedef struct
{
	uint64_t base;
	uint64_t len;
}_fetcher_command_ct_load_t;

typedef struct
{
	uint64_t ins_start;
	uint64_t ins_len;
}_fetcher_command_ins_set_t;

typedef struct
{
	uint64_t base;
	uint64_t len;

}_fetcher_command_ins_load_t;

typedef struct
{
	uint64_t imm_start;
	uint64_t imm_len;
}_fetcher_command_imm_set_t;

typedef struct
{
	uint64_t base;
	uint64_t len;
}_fetcher_command_imm_load_t;

struct wcpu_part_signal_FETCHER_COMMAND
{

	_fetcher_command_type_t type;

	union
	{
		_fetcher_command_cd_swap_t cd_swap;
		_fetcher_command_ct_load_t ct_load;
		_fetcher_command_ct_set_t ct_set;
		_fetcher_command_ins_set_t ins_set;
		_fetcher_command_imm_set_t imm_set;
		_fetcher_command_imm_load_t imm_load;
		_fetcher_command_ins_load_t ins_load;
	}cmd;

};



#endif