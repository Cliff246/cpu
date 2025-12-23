
#include "vm_alu.h"
#include "vm_mem.h"

#include "vm_jmp.h"
#include "vm_sys.h"
#include "vm_op.h"
#include "vm_path.h"
#include "flags.h"

#define OP_NAME(path, subpath) path ## _ ## subpath
#define OP_RUN(path, subpath) vm_ ## path ## _ ## subpath ## _fn
#define OP_FILL(path, sp) [OP_NAME(path, sp)] = \
	{						\
		.string = #sp,	\
		.subpath = OP_NAME(path, sp), \
		.run = OP_RUN(path, sp),		\
	}\

vm_path_t vm_wcpu_ops[] =
{
	[PATH_ALU] =
	{
		.path = PATH_ALU,
		.string = "alu",
		.ops =
		{
			OP_FILL(ALU, ADD),
			OP_FILL(ALU, SUB),
			OP_FILL(ALU, SUBU),
			OP_FILL(ALU, AND),
			OP_FILL(ALU, OR),
			OP_FILL(ALU, XOR),
			OP_FILL(ALU, SRL),
			OP_FILL(ALU, SRA),
			OP_FILL(ALU, SLL),
			OP_FILL(ALU, DIV),
			OP_FILL(ALU, REM),
			OP_FILL(ALU, MUL),
			OP_FILL(ALU, DIVU),
			OP_FILL(ALU, MULHI),
			OP_FILL(ALU, MULU),
			OP_FILL(ALU, MULUS),
			OP_FILL(ALU, NOT),
			OP_FILL(ALU, CLT),
			OP_FILL(ALU, CEQ),
			OP_FILL(ALU, CNE),
			OP_FILL(ALU, CLTU),
			OP_FILL(ALU, CLE),


		}
	},
	[PATH_MEM] =
	{
		.path = PATH_MEM,
		.string = "mem",
		.ops =
		{
			OP_FILL(MEM, STI),
			OP_FILL(MEM, LDI),
			OP_FILL(MEM, PUSH),
			OP_FILL(MEM, POP),
			OP_FILL(MEM, INCSP),
			OP_FILL(MEM, DECSP),
			OP_FILL(MEM, LDS),
			OP_FILL(MEM, STS),
			OP_FILL(MEM, SP),
			OP_FILL(MEM, SFP),

		}
	},
	[PATH_JMP] =
	{
		.path = PATH_JMP,
		.string = "jmp",
		.ops =
		{
			OP_FILL(JMP, JMP),
			OP_FILL(JMP, BNE),
			OP_FILL(JMP, BEQ),
			OP_FILL(JMP, CALL),
			OP_FILL(JMP, RET),
			OP_FILL(JMP, BLT),
			OP_FILL(JMP, BLE),
			OP_FILL(JMP, BLTU),
			OP_FILL(JMP, BLEU),
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
