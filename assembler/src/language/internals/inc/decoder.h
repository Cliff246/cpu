#ifndef __DECODER__
#define __DECODER__


int get_register(char *keyword);

int get_path(char *keyword);
int get_subpath(int path,char *keyword);

int get_alu_subpath(char *keyword);
int get_mem_subpath(char *keyword);
int get_jmp_subpath(char *keyword);
int get_sys_subpath(char *keyword);

#endif