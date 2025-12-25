
#include "vm_alu.h"
#include "vm_mem.h"

#include "vm_jmp.h"
#include "vm_sys.h"
#include "vm_op.h"
#include "vm_path.h"
#include "flags.h"

#define OP_RUN(path, subpath) vm_ ## path ## _ ## subpath ## _fn
#define OP_FILL(PU, PL, SU, SL, N, KEY,V,OP,EQU, DESC, SIG ) [N] = \
	{						\
		.string = #KEY,	\
		.subpath = N, \
		.run = OP_RUN(PU, SU),		\
	},\

vm_path_t vm_wcpu_ops[] =
{
	[PATH_ALU] =
	{
		.path = PATH_ALU,
		.string = "alu",
		.ops =
		{
			WCPU_SUBPATH_ALU_LIST(OP_FILL)



		}
	},
	[PATH_MEM] =
	{
		.path = PATH_MEM,
		.string = "mem",
		.ops =
		{
			WCPU_SUBPATH_MEM_LIST(OP_FILL)


		}
	},
	[PATH_JMP] =
	{
		.path = PATH_JMP,
		.string = "jmp",
		.ops =
		{
			WCPU_SUBPATH_JMP_LIST(OP_FILL)

		}

	},
	[PATH_SYS] =
	{
		.path = PATH_SYS,
		.string = "sys",

		.ops = {

		},

	}

};
