
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "decoder.h"
#include "commons.h"
#include "error.h"
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>


#include "hashmap.h"
int step = 0;
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
			context->instructions = REALLOC(context->instructions, context->instructions_alloc, uint32_t);
		}
	}
			
	context->instructions[index] = inst;		
	if(index >= context->instructions_len)
		context->instructions_len = index + 1;
}


void set_context_immedates(context_t *context, size_t index, int64_t imm)
{
	if(index > context->immedates_len)
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
			context->immedates = REALLOC(context->immedates, context->immedates_alloc, int64_t);
		}
	}
	context->immedates[index] = imm;		
	if(index >= context->immedates_len)
	{
		context->immedates_len = index + 1;
	}
}


int *collect_lines(char *content, size_t length)
{
	size_t alloc = 100;
	int *lines = (int *)CALLOC(alloc, int); 
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
				
				lines = REALLOC(lines, alloc, int);
			}
			lines[index++] = i;	
			
		}
		
	}
	
	if(index >= alloc)
	{
		alloc += 10;
		lines = REALLOC(lines, alloc, int);
	}	
	lines[index] = -1;
	return lines;

}

int *collect_references(char *content, size_t length)
{

	size_t alloc = 100;
	int *references = (int *)CALLOC(alloc, int); 
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
					references = REALLOC(references, alloc, int);
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
		references = REALLOC(references, alloc, int);
	}	
	references[index] = -1;
	return references;
}

void test(char *content, size_t length)
{
	int *lines = collect_lines(content, length);
	for(int *index = lines; *index >= 0; index++)
	{
		//printf("%d\n", *index);
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
	char **splitlines = (char **)CALLOC(sl_alloc, char *);
	if(!splitlines)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);	
	}

	size_t r_alloc = alloc_init;	
	reference_t *refs = (reference_t *)CALLOC(r_alloc, reference_t); 
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
		if(delta == 0)
		{
			continue;
		}
		//buffer for strdup
		//printf("%d\n", delta);
		char current[delta + 1];
		memset(current, 0, delta + 1);
		memcpy(current, content + last + 1, delta - 1);

		if(freferences[iref] == last) 
		{
			//resize 
			if(iref >= r_alloc)
			{
				r_alloc *= 2;
				refs = (reference_t *)REALLOC(refs, r_alloc, reference_t);	
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
			splitlines = REALLOC(splitlines, sl_alloc, char *);

		splitlines[ilines] = split;
		//printf("%s\n", splitlines[ilines]);
		last = *line;
		
	}	
	size_t nlines = ilines; 
	size_t nrefs = iref;
	
	//for(int i = 0; i < nrefs; ++i)
	//{
		//printf("%s\n", refs[i].str);
		//printf("%s\n", splitlines[refs[i].line]);
	//}
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
	invalid,
	reference,
	comment,
	instruction,
}codeline_t;


void free_reference(void *ref)
{
	reference_t *ptr = (reference_t *)ref;
	free(ptr->str);
	free(ptr);
}


struct asm_lines
{
	int current_ref;
	int icount;	
	hashtable_t *table;

	codeline_t *codeline; 
};


struct asm_lines determine_lines(context_t *context, struct asm_first_stage_context *ptr)
{
	struct asm_lines al; 
	int icount = 0;
	int current_ref = 0;
	codeline_t *codeline = (codeline_t *)CALLOC(ptr->sp_len, codeline_t);

	hashtable_t *table = new_hash_table(ptr->ref_len, free_reference);
	
	for(int entry = 0; entry < ptr->ref_len; entry++)
	{
		printf("refs[%d].address = %llu\n", entry, ptr->refs[entry].address );
		addto_hash_table(table, ptr->refs[entry].str, &ptr->refs[entry]);
	}

	for(int i = 0; i < ptr->sp_len; ++i)
	{
		if(ptr->sp[i] == NULL)
			continue;
		if(ptr->sp[i][0] == 0)
			codeline[i] = comment;
		if(ptr->refs[current_ref].line == i)
		{	
			
			codeline[i] = reference;
			ptr->refs[current_ref].address = icount;
			current_ref++;

		}
		else if(ptr->sp[i][0] == '\t' )
		{
					
			codeline[i] = instruction;
			icount++;
			//printf("codeline %d\n", i);
			printf("%x: %s\n",i , ptr->sp[i]);
		}
		else if(ptr->sp[i][0] == '#')
		{
			codeline[i] = comment;
		}
		
		else
		{
			codeline[i] = invalid;
		}
	}
	
	al.table = table;
	al.codeline = codeline;
	al.current_ref = current_ref;
	al.icount = icount;
	return al;
}

uint64_t second_stage(context_t *context, struct asm_first_stage_context *ptr)
{

	struct asm_lines al = determine_lines(context, ptr);

	codeline_t *codeline = al.codeline; 
	hashtable_t *table = al.table; 	
	
	const int icount = al.icount;

	const uint64_t cd_table_len = (icount / 128) + 1;
	
	inst_t *inary = CALLOC(icount, inst_t);
	uint64_t *immary = CALLOC(icount, uint64_t); 
	printf("icount: %d %d\n", icount, icount / 128 + 1);
	uint64_t *cd_table_offsets = CALLOC(cd_table_len, uint64_t);
	//reusing this
	
	const uint64_t instrptr_start = (cd_table_len * 2) + 6;
	printf("instr_ptr start:%llu\n", instrptr_start);
	
	//immediate index
	int immind = 0;
	//instruction index
	int iind = 0;
	//code description count
	int cd_offset_count = 0;
	for(int x = 0; x < ptr->sp_len; ++x)
	{
		if(codeline[x] == instruction)
		{
			if(iind % 128 == 0)
			{
				cd_table_offsets[cd_offset_count++] = immind;
				printf("immoffsets[%d]=%llu\n",cd_offset_count - 1 ,cd_table_offsets[cd_offset_count-1]);
			}
			
			inst_t temp = create_instruction(ptr->sp[x], x);
			
			if(temp.immflag || temp.immref)
			{
				if(temp.immref != NULL)
				{
					reference_t *ref = (reference_t *)getdata_from_hash_table(table, temp.immref); 
					if(ref == NULL)
					{
						printf("ref key '%s' not found in table\n", temp.immref);
						print_hash_table(table);
						exit(1);
					}
					temp.imm = ref->address;
					printf("address = %llu\n", ref->address);
					
				}
				
				set_context_immedates(context, immind++, temp.imm);
			}
			inary[iind] = temp;
			//printf("instruction is: at: %.4x %.8x: ", iind, encode_inst(&temp));
			//print_inst(&temp);
			set_context_instruction(context, iind, encode_inst(&temp));
						
			//printf("imm: %d %llu\n", immind, temp.imm);

			//printf("trying\n");
			iind++;
		}
	}	
	
	context->table = cd_table_offsets;
	context->table_len = cd_table_len; 	

	int table_total_len = context->table_len * 2;
	int code_size = context->immedates_len + context->instructions_len;
	
	int total_size = code_size + table_total_len + 6;
	return total_size;
}


void final_stage(context_t *context, int length, char *file_name)
{
	
	uint64_t *bin = (uint64_t *)CALLOC(length, uint64_t);
	
	
	const uint64_t ct_actual_len = context->table_len * 2;
	//table
	int offset = 0;
	bin[offset = 0] = 6;
	bin[offset = 1] = ct_actual_len;
	//code table actual length
	//inst ptr
	const uint64_t instr_ptr = (context->table_len * 2) + 6;
	const uint64_t instr_len = context->instructions_len; 
	bin[offset = 2] = instr_ptr; 
	bin[offset = 3] = instr_len;
	//imm ptr
	const uint64_t imm_ptr = (context->table_len * 2)+ context->instructions_len + 6;
	const uint64_t imm_len = context->immedates_len;
	bin[offset = 4] = imm_ptr;
	bin[offset = 5] = imm_len;
	//just buffering by 1
	offset = offset + 1;
	for(int table = 0; table < context->table_len; table++, offset+=2)
	{
		printf("table[%d]=%llu\n",table, context->table[table]);
		bin[offset] = (64 * table);
		bin[offset + 1] =  context->table[table];
	}
	for(int c = 0; c < context->instructions_len; ++c)
	{
		//inst_t i = decode_inst(context->instructions[c]);
		//print_inst(&i);
		bin[offset++] = context->instructions[c];
	}
	for(int i = 0; i <= context->immedates_len; ++i)
	{
		bin[offset++] = context->immedates[i];
	}
	if(offset <= length)
	{
		//fuck
		ERR(ERANGE, "offset is off");	
		exit(1);
	}
	FILE *fp = fopen(file_name, "w");
	if(fp == NULL)
	{
		ERR(EBADF, "could not load file");
		exit(1);
	}
	char *c = (char *)bin;
	for(int i = 0; i < offset * sizeof(int64_t); i++)
	{
		if(i % sizeof(int64_t) == 0)
		{

			printf("%.3d: %.8llx %llu\n", i/8, bin[i / 8], bin[i / 8]);
		}
		fputc(c[i], fp);
	}
	fclose(fp);
}


void assemble(char *content, size_t length, char *file_name)
{
	
	static context_t context = {0};
	const size_t alloc_init = 100;
	printf("step%d\n", step);
	context.immedates = CALLOC(alloc_init, uint64_t); 
	context.instructions = CALLOC(alloc_init, uint32_t); 
	printf("step%d\n", step);
	context.immedates_alloc = alloc_init;
	context.instructions_alloc = alloc_init;
	printf("step%d\n", step);
	struct asm_first_stage_context stage1 = first_stage(content, length);	
	printf("step%d\n", step);
	uint64_t size = second_stage(&context, &stage1);
	printf("step%d\n", step);
	final_stage(&context, size, file_name);
}
