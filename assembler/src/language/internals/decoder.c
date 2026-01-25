#include "flags.h"
#include "decoder.h"
#include "eerror.h"
#include "commons.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strtools.h"
#include "parser.h"
#include <stdbool.h>
#include <sys/types.h>

#define REGISTER_ALIAS_MAX 2
#define REGISTER_ALIAS_KEYSIZE 4

#define SUBPATH_KEYCODE(PATH, S) PATH ## _ ## S
#define PATH_KEYCODE(X) PATH_ ## X
#define SUBPATH_MNEMONIC(PU, PL, SU, SL, N, KEY,V,OP,EQU, DESC, SIG ) [N] = \
{							   \
	.path =  PATH_KEYCODE(PU),\
	.subpath = N, \
	.str = #KEY,		\
	.baseimpl = OP,		\
	.equation = EQU,	\
	.desc = DESC,	\
},			\


ASM_mnemonic_t ASM_mnemonics_list[] =
{
	[PATH_ALU] =
	{
		.path = PATH_ALU,
		.str = "alu",
		.subpaths =
		{
			WCPU_SUBPATH_ALU_LIST(SUBPATH_MNEMONIC)
		},
	},
	[PATH_MEM] =
	{
		.path = PATH_MEM,
		.str = "mem",
		.subpaths =
		{
			WCPU_SUBPATH_MEM_LIST(SUBPATH_MNEMONIC)

		},
	},
	[PATH_JMP] =
	{
		.path = PATH_JMP,
		.str = "jmp",
		.subpaths =
		{
			WCPU_SUBPATH_JMP_LIST(SUBPATH_MNEMONIC)

		},
	},
	[PATH_SYS] =
	{
		.path = PATH_SYS,
		.str = "sys",
		.subpaths =
		{
			WCPU_SUBPATH_SYS_LIST(SUBPATH_MNEMONIC)

		},
	},
	[PATH_FPU] =
	{
		.path = PATH_FPU,
		.str = "fpu",
		.subpaths =
		{
			WCPU_SUBPATH_FPU_LIST(SUBPATH_MNEMONIC)
		},
	}
};

#define REGISTER_ALIAS_STRING(X)
#define REGISTER_ALIAS_ENCODE(A, B, C, D, E) [A] = { .count = B, .alias = {#C, #D, #E}},

struct ASM_register_alias
{
	int count;
	char *alias[3];
};

struct ASM_register_alias ASM_registers_aliases[64] =
{
	REGISTER_NAMES(REGISTER_ALIAS_ENCODE)
};


int ASM_get_register_alias(char *keyword)
{
	for(int i = 0; i < 64; ++i)
	{
		struct ASM_register_alias *alias = &ASM_registers_aliases[i];
		for(int s = 0; s < alias->count; ++s)
		{
			if(!strcmp(keyword, alias->alias[s]))
			{
				return i;
			}

		}
	}
	return -1;
}


int ASM_get_subpath(int path, char *keyword)
{
	for(int i = 0; i < 128; ++i)
	{
		if(!strcmp(keyword, ASM_mnemonics_list[path].subpaths[i].str))
		{
			return ASM_mnemonics_list[path].subpaths[i].subpath;
		}
	}
	return -1;
}





int get_sys_subpath(char *keyword)
{
	return ASM_get_subpath(PATH_SYS, keyword);



}



int get_path(char *keyword)
{


	const char *const pathwords[] = {
		"alu",
		"mem",
		"jmp",
		"sys",
		"fpu",
	};

	const int pathvalue[] = {
		PATH_ALU,
		PATH_MEM,
		PATH_JMP,
		PATH_SYS,
		PATH_FPU,

	};
	int code = determine_code(keyword, pathwords, ARYSIZE(pathwords));
	if(code == -1)
	{
		printf("not a path code\n");
		return -1;
	}
	return pathvalue[code];
}




int get_subpath(int path, char *keyword)
{

	if(path < 0 || path > ARYSIZE(ASM_mnemonics_list))
	{
		return -1;
	}

	int val = ASM_get_subpath(path, keyword);
}


bool valid_reference(char **str, int length)
{
	//
	if(str == NULL)
		return false;
	if(length < 0)
		return false;


	return true;
}



bool valid_instruction(char **str, int length)
{
	if(str == NULL)
		return false;
	if(length < 0)
		return false;





	return true;
}




