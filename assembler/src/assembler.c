
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include <stdint.h>
#include <stdbool.h>




void set_context_instruction(context_t *context, size_t index, uint32_t inst)
{
	if(index > context->instructions_len)
	{
		size_t diff_needed = index -  context->instructions_len;	
		bool changed = false;
	   	if(	context->instructions_alloc < index)
		{
			//maybe infinite 
			changed = true;
			while(context->instructions_alloc < index)
			{
				context->instructions_alloc *= 2;
			}
		}

		
		if(changed)
		{
			uint32_t *temp = realloc(context->instructions, context->instructions_alloc * sizeof(uint32_t));
			if(temp == NULL)
			{
				//die
				printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
				exit(1);	
			}
			context->instructions = temp;
		}
	}
			
	context->instructions[index] = inst;		
	if(index > context->instructions_len)
		context->instructions_len = index;
}


void set_context_immedates(context_t *context, size_t index, uint64_t imm)
{
	if(index > context->instructions_len)
	{
		size_t diff_needed = index -  context->immedates_len;	
		bool changed = false;
	   	if(	context->immedates_alloc < index)
		{
			//maybe infinite 
			changed = true;
			while(context->immedates_alloc < index)
			{
				context->immedates_alloc *= 2;
			}
		}

		
		if(changed)
		{
			uint64_t *temp = realloc(context->immedates, context->immedates_alloc * sizeof(uint64_t));
			if(temp == NULL)
			{
				//die
				printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
				exit(1);	
			}
			context->immedates = temp;
		}
	}
			
	context->immedates[index] = imm;		
	if(index > context->instructions_len)
		context->instructions_len = index;

}


int *collect_lines(char *content, size_t length)
{
	size_t alloc = 100;
	int *lines = (int *)calloc(alloc, sizeof(int)); 
	if(!lines)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}

	int index = 1;
	for(int i = 0; i < length; ++i)
	{
		if(content[i] == '\n')
		{
			if(index + 1 >= alloc)
			{
				alloc *= 2;
				int *temp = realloc(lines, alloc * sizeof(int));	
				if(!temp)
				{
					printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
					exit(1);
				}
				lines= temp;
			}
			lines[index++] = i;	
			
		}
		
	}
	
	if(index >= alloc)
	{
		alloc += 10;
		int *temp = realloc(lines, alloc * sizeof(int));	
		if(!temp)
		{
			printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
			exit(1);
		}
		lines = temp;
	}	
	lines[index] = -1;
	return lines;

}

int *collect_references(char *content, size_t length)
{

	size_t alloc = 100;
	int *references = (int *)calloc(alloc, sizeof(int)); 
	if(!references)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}

	int last_line = 0;
	int index = 0;
	bool hasref = false;	
	for(int i = 0; i < length; ++i)
	{
		if(content[i] == '\n')
		{
			if(hasref == true)
			{
				if(index + 1 == alloc)
				{
					alloc *= 2;
					int *temp = realloc(references, alloc * sizeof(int));	
					if(!temp)
					{
						printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
						exit(1);
					}
					references = temp;
				}
				references[index++] = last_line;	
			}
			last_line = i;
			hasref = false;
		}
		else if(content[i] == ':')
		{
			hasref = true;	
		}
	}
	
	if(index + 1 == alloc)
	{
		alloc += 10;
		int *temp = realloc(references, alloc * sizeof(int));	
		if(!temp)
		{
			printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
			exit(1);
		}
		references = temp;
	}	
	references[index] = -1;
	return references;
}

void test(char *content, size_t length)
{
	int *lines = collect_lines(content, length);
	for(int *index = lines; *index >= 0; index++)
	{
		printf("%d\n", *index);
	}
}

struct asm_first_stage_context
{
	//split lines
	char **sp;
	//refs 
	reference_t *refs;
	
	size_t sp_len;
	size_t ref_len;	
};


struct asm_first_stage_context first_stage(char *content, size_t length)
{
	size_t alloc_init = 100;
	int *flines = collect_lines(content, length);
	//yes inefficent
	int *freferences = collect_references(content, length);
	
	//reserve buffer
	size_t sl_alloc = alloc_init;	
	//all lines
	char **splitlines = (char **)calloc(sl_alloc, sizeof(char *));
	if(!splitlines)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);	
	}

	size_t r_alloc = alloc_init;	
	reference_t *refs = (reference_t *)calloc(r_alloc, sizeof(reference_t)); 
	if(!refs)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);	

	}
	//go through and build sets
	int last = 0, ilines = 0, iref = 0;
	for(int *line = (flines + 1); *(line) >= 0;  ilines++, line++)
	{
		//reference counting		
		// get current delta	
		int delta = (*line) - last; 
		//buffer for strdup
		char current[delta + 1];
		memset(current, 0, delta + 1);
		memcpy(current, content + last + 1, delta - 1);
		if(freferences[iref] == last) 
		{
			//resize 
			if(iref >= r_alloc)
			{
				r_alloc *= 2;
				reference_t *temp = (reference_t *)realloc(refs, r_alloc * sizeof(reference_t));	
				if(!temp)
				{
					printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
					exit(1);
				}
				refs = temp;
			}
			
			char *after = strchr(current, ':');
			if(!after)
			{
				
				printf("line was lost %d\n", ilines);	
				exit(1);
			
			}
			int length = after - current;
			//saftey
			char before_buffer[length + 1];
			memset(before_buffer, 0, length + 1);
			memcpy(before_buffer, current, length);	
			if(strchr(before_buffer, ' '))
			{
				//should die here
				refs[iref].str = strdup(before_buffer); 
				refs[iref].line = ilines; 			
			}	
			else
			{
				refs[iref].str = strdup(before_buffer); 
				refs[iref].line = ilines; 			
			}
			iref++;
		}

		//strdup
		char *split =  strdup(current);
		for(char *split_ptr = split; *split_ptr != 0; split_ptr++)
		{
			*split_ptr = (*split_ptr == '\n')? 0: *split_ptr;

		}
		//save it
		if(ilines >= sl_alloc)
		{
			sl_alloc *= 2;
			char **temp = (char **)realloc(splitlines, sl_alloc * sizeof(char *));	
			if(!temp)
			{
				printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
				exit(1);
			}
			splitlines = temp;

		}
		splitlines[ilines] = split;
		//printf("%s\n", splitlines[ilines]);
		last = *line;
		
	}	
	size_t nlines = ilines; 
	size_t nrefs = iref;
	
	for(int i = 0; i < nrefs; ++i)
	{
		//printf("%s\n", refs[i].str);
		printf("%s\n", splitlines[refs[i].line]);
	}
	free(freferences);
	free(flines);

	struct asm_first_stage_context afsc = {
		.refs = refs,
		.sp = splitlines,
		.ref_len = nrefs,
		.sp_len = nlines	
	};
	return afsc; 
}


typedef enum
{
	instruction,
	reference,
	comment,
	invalid,
}codeline_t;

void second_stage(struct asm_first_stage_context *ptr)
{
	int current_ref = 0;
	codeline_t *codeline = (codeline_t *)malloc(sizeof(codeline_t) * ptr->sp_len);
	for(int i = 0; i < ptr->sp_len; ++i)
	{
		if(ptr->sp[i][0] == 0)
			codeline[i] = comment;
		if(ptr->refs[current_ref].line == i)
		{	
			codeline[i] = reference;
		}
		else if(ptr->sp[i][0] == '\t' )
		{
			codeline[i] = instruction;
		}
		else if(ptr->sp[i][0] == '#')
		{
			codeline[i] = comment;
		}
	}





}


void assemble(char *content, size_t length)
{
	
	static context_t context = {0};
	const size_t alloc_init = 100;
	context.immedates = calloc(alloc_init, sizeof(uint64_t)); 
	context.instructions = calloc(alloc_init, sizeof(uint32_t)); 
	if(!context.immedates)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}
	if(!context.instructions)
	{

		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}	
	context.immedates_alloc = alloc_init;
	context.instructions_alloc = alloc_init;

	struct asm_first_stage_context stage1 = first_stage(content, length);	

}
