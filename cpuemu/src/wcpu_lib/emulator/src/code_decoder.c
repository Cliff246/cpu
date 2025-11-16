#include "code_decoder.h"
#include "core.h"
#include "coreutils.h"
#include "flags.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

char path_codes[16][PATH_STR_SIZE] =
{
	[PATH_ALU] = "alu",
	[PATH_MEM] = "mem",
	[PATH_JMP] = "jmp",
	[PATH_SYS] = "sys",
	[PATH_FPU] = "fpu",

};













	   //increment stack ptr


char subpath_codes[16][0xff][PATH_STR_SIZE] =
{
	[PATH_ALU] =
	{
		[ALU_ADD  ]	=	"add",
		[ALU_SUB ]	=	"sub",
		[ALU_SUBU]	=	"subu",
		[ALU_AND ]	=	"and",
		[ALU_OR  ]	=	"or",
		[ALU_XOR ]	=	"xor",
		[ALU_SLL ]	=	"sll",
		[ALU_SRL ]	=	"srl",
		[ALU_SRA ]	=	"sra",
		[ALU_DIV ]	=	"div",
		[ALU_MUL ]	=	"mul",
		[ALU_REM ]	=	"rem",
		[ALU_MULHI]	=	"mulhi",
		[ALU_MULU]	=	"mulu",
		[ALU_MULUS]	=	"mulus",
		[ALU_DIVU]	=	"divu",
		[ALU_NOT ]	=	"not",
		[ALU_CLE ]	=	"cle",
		[ALU_CLT ]	=	"clt",
		[ALU_CLTU]	=	"cltu",
		[ALU_CNE ]	=	"cne",
		[ALU_CEQ ]	=	"ceq",
	},
	[PATH_MEM] =
	{
		[MEM_LDI ]= "ld",
		[MEM_STI ]=	"st",
		[MEM_PUSH]=	"push",
		[MEM_POP ]=	"pop",
		[MEM_SP  ]=	"sp",
		[MEM_SFP ]=	"sfp",
		[MEM_LDS ]=	"lds",
		[MEM_STS ]=	"sts",
		[MEM_INCSP]="isp",
		[MEM_DECSP]="dsp",

	},
	[PATH_JMP] =
	{
		[JP_JMP] = "jmp",
		[JP_BNE] = "bne",
		[JP_BLT] = "blt",
		[JP_BEQ] = "beq",
		[JP_BLE] = "ble",
		[JP_CALL]= "call",
		[JP_RET] = "ret",
		[JP_BLEU]= "bleu",
		[JP_BLTU]= "bltu",
	},
	[PATH_SYS] =
	{
		[SYS_BREAK] = "break",
	},
	[PATH_FPU] =
	{

	}
};


#define INT_TO_STR_SIZE 20
void int_to_str(int val, char buf[INT_TO_STR_SIZE])
{
	memset(buf, 0, INT_TO_STR_SIZE);
	int i = 0;
	while(val != 0)
	{
		int rem = val % 10;
		buf[i++] = '0' + rem;
		val /= 10;
	}
	reverse(buf);

}

int path_to_string(char buffer[PATH_STR_SIZE], int code)
{
	if(code >= 16 || code < 0)
	{
		return -1;
	}

	strcpy(buffer, path_codes[code]);
	return 0;
}

int subpath_to_string(char buffer[PATH_STR_SIZE], int path, int subpath)
{
	//printf("subpath %d %s\n", subpath, (char *)subpath_codes[path][subpath]);
	strcpy(buffer, subpath_codes[path][subpath]);
	return 0;
}




int register_to_string(char buffer[REG_STR_SIZE], unsigned reg, char selflag)
{
	buffer[0] = (selflag)?'#':'x';
	if(reg == 0)
	{
		buffer[1] = '0';
		return 0;
	}

	char numbuf[INT_TO_STR_SIZE];
	int_to_str(reg, numbuf);

	buffer[1] = numbuf[0];

	buffer[2] = numbuf[1];
	return 0;
}

char *operation_to_text_static_buffer(operation_t *op)
{
	inst_t inst;
	if(op->inst.decoded == false)
	{
		uint32_t raw = op->inst.raw;
		//printf("decode op in text %d\n", raw);
		inst = decode_inst(raw);
	}
	else
	{
		inst = get_inst_from_op(op);
	}
	//print_inst(&inst);


	static char buffer[1000];
	memset(buffer, 0, sizeof(buffer)/sizeof(*buffer));
	char pathbuf[PATH_STR_SIZE] = {0};
	path_to_string(pathbuf, inst.path);
	char subpathbuf[PATH_STR_SIZE] = {0};
	subpath_to_string(subpathbuf, inst.path, inst.subpath);
	char rdbuf[REG_STR_SIZE] = {0};
	register_to_string(rdbuf, inst.rs1, 0);
	char rs1buf[REG_STR_SIZE] = {0};
	register_to_string(rs1buf, inst.rs2, 0);
	char rs2buf[REG_STR_SIZE] = {0};
	register_to_string(rs2buf, inst.rs3, inst.selflag);
	char static_accflag[2][2] = {
		[0] = "",
		[1] = "!"
	};
	char *accflag = (inst.accflag)? static_accflag[1]:static_accflag[0];
	if(inst.immflag)
	{

		sprintf(buffer, "%s.%s%s %s, %s, %s, %lld", pathbuf, subpathbuf,accflag, rdbuf, rs1buf, rs2buf, op->imm.imm);
	}
	else
	{

		sprintf(buffer, "%s.%s%s %s, %s, %s", pathbuf, subpathbuf,accflag, rdbuf, rs1buf, rs2buf);
	}
	return buffer;

}


char *convert_operation_to_text(operation_t *op)
{


	char *string = calloc(4 + 8 + 2 + 3 + 2 + 3 + 5 + 20 + 1, sizeof(char));

	strcpy(string, operation_to_text_static_buffer(op));
	return string;
}
