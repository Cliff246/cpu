#ifndef __WS_MOD_FUNCTION_HEADER__
#define __WS_MOD_FUNCTION_HEADER__

#define MOD_FUNCTION_NAME_SIZE 30




typedef struct WS_MOD_function
{
	int version;
	//common runtime tag
	int crtt;
	char name[MOD_FUNCTION_NAME_SIZE];


}MOD_function_t;

#endif