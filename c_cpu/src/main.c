#include "core.h"
#include "memory.h"

int main(int argc, char *argv[])
{
	
	init_components();	

	for(int i = 0; i < 100; ++i)
	{
		step_cpu();
	}	
}
