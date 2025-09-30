#ifndef __ASM_STR_STUFF__
#define __ASM_STR_STUFF__


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int split_str(const char *string, char ***ptr, const char *delims);
char *clear_whitespace(char *str);

int determine_code(char *keyword, const char *const mnemonics[], int length);



bool valid_reference(char **str, int length);
bool valid_instruction(char **str, int length);



#endif

