#ifndef __DECODER__
#define __DECODER__

#define MNEMONIC_KEYSIZE 20


typedef struct subpath_mnemonic
{
	int path;
	int subpath;
	char str[MNEMONIC_KEYSIZE];
	char desc[100];
	char baseimpl[10];
	char opp[10];
	char equation[100];
	int sign;
}ASM_subpath_mnemonic_t;






typedef struct mnemonic
{
	int path;
	char str[MNEMONIC_KEYSIZE];
	ASM_subpath_mnemonic_t subpaths[128];
}ASM_mnemonic_t;

extern ASM_mnemonic_t ASM_mnemonics_list[8];


extern const char *const mem_mnemonics[];
extern const char *const alu_mnemonics[];
extern const char *const jmp_mnemonics[];
extern const char *const sys_mnemonics[];


int get_register(char *keyword);

int get_path(char *keyword);
int get_subpath(int path,char *keyword);

int get_alu_subpath(char *keyword);
int get_mem_subpath(char *keyword);
int get_jmp_subpath(char *keyword);
int get_sys_subpath(char *keyword);

#endif