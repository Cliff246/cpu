#include "core.h"
#include "cpumemory.h"
#include "flags.h"

#include "stdio.h"
#include <stdlib.h>




void fill_binary(uint64_t *bin, size_t length)
{

	for(size_t i = 0; i < length; ++i)
	{

		memory_write(components.mem, i, bin[i] );
	}
}

size_t file_len(FILE *fp)
{
	if(!fp)
		return 0;

	size_t current = ftell(fp);

	fseek(fp, 0, SEEK_END);
	size_t address = ftell(fp);

	fseek(fp, current, SEEK_SET);
	return address;
}


void load_file(const char *file_name)
{
	FILE *fp = fopen(file_name, "rb");
	if(fp == NULL)
	{
		printf("file: %s not openable\n", file_name);
		exit(1);

	}
	size_t len = file_len(fp);

	char *bytes = (char *)calloc(len, sizeof(char));

	fread(bytes, 8, len / 8, fp);
	uint64_t *bin = (uint64_t *)bytes;

	fill_binary(bin, len / sizeof(uint64_t));



}


int main(int argc, char *argv[])
{

	if(argc == 1)
	{
		printf("not enough arguments\n");
	}
	else if(argc == 2)
	{
		init_components();

		load_file(argv[1]);


		startup_cpu();

		for(int i = 0; i < (1000); ++i)
		{

			step_cpu();
		}
		print_regs();
	}
	else
	{
		printf("%d arguments I need to build the arg parser\n", argc);
		exit(1);
	}

}
