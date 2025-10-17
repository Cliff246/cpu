
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "decoder.h"
#include "commons.h"
#include "strtools.h"
#include "error.h"
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>


#include "hashmap.h"
#include <flags.h>


int step = 0;


void set_binary_instruction(asmbinary_t *binary, size_t index, uint32_t inst)
{
	if(index > binary->instructions_len)
	{
		size_t diff_needed = index -  binary->instructions_len;
		bool changed = false;
	   	if(binary->instructions_alloc < index)
		{
			//maybe infinite
			changed = true;
			while(binary->instructions_alloc < index)
			{
				binary->instructions_alloc *= 2;
			}
		}


		if(changed)
		{
			binary->instructions = REALLOC(binary->instructions, binary->instructions_alloc, uint32_t);
		}
	}

	binary->instructions[index] = inst;
	if(index >= binary->instructions_len)
		binary->instructions_len = index + 1;
}


void set_binary_immedates(asmbinary_t *binary, size_t index, int64_t imm)
{
	if(index > binary->immedates_len)
	{
		size_t diff_needed = index -  binary->immedates_len;
		bool changed = false;
	   	if(	binary->immedates_alloc < index)
		{
			//maybe infinite
			changed = true;
			while(binary->immedates_alloc < index)
			{
				binary->immedates_alloc *= 2;
			}
		}


		if(changed)
		{
			binary->immedates = REALLOC(binary->immedates, binary->immedates_alloc, int64_t);
		}
	}
	binary->immedates[index] = imm;
	if(index >= binary->immedates_len)
	{
		binary->immedates_len = index + 1;
	}
}






void test(char *content, size_t length)
{
	int *lines = collect_segments(content, length);
	for(int *index = lines; *index >= 0; index++)
	{
		printf("%d\n", *index);
	}
}

segment_type_t get_segment_type(char *str, size_t max)
{
	char *buffer = CALLOC(max + 1, char);
	memcpy(buffer, str, max);
	const char *types[] = {
		".text",
		".realloc",
		".data"
	};
	segment_type_t translate[] = {
		SEG_TEXT,
		SEG_REALLOC,
		SEG_DATA
	};
	printf("%s\n", buffer);
	for(int i = 0; i < ARYSIZE(types); ++i)
	{
		int cmp = strcmp(buffer, types[i]);
		printf("%s %s cmp: %d %d\n", buffer, types[i], cmp, i);
		if(cmp == 0)
		{
			return translate[i];
		}
	}
	return SEG_INVAL;



}

void first_stage(context_t *context, char *content, size_t length)
{
	size_t alloc_init = 100;
	int *flines = collect_lines(content, length);
	//yes inefficent
	int *freferences = collect_references(content, length);
	int *fsegments = collect_segments(content, length);
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
	size_t seg_alloc = alloc_init;
	segment_t *segs = (segment_t *)CALLOC(seg_alloc, segment_t);

	//go through and build sets
	int last = 0, ilines = 0, iref = 0, iseg = 0;
	for(int *line = (flines); *(line) >= 0;  ilines++, line++)
	{
		int delta = (*line) - last;
		if(delta == 0)
		{
			continue;
		}
		//buffer for strdup
		//printf("%d\n", delta);
		char current[delta + 1];
		memset(current, 0, delta + 1);

		if(ilines == 0)
		{
			memcpy(current, content, delta );
		}
		else
		{
		//reference counting
		// get current delta


			memcpy(current, content + last + 1, delta - 1);
		}
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
		else if(fsegments[iseg] == last)
		{
			printf("segment '%s' %d\n",current,fsegments[iseg]);
			if(iseg == 0)
			{
				segs[0].start = ilines;
			}
			else
			{
				segs[iseg - 1].range = ilines - segs[iseg - 1].start;
			}
			if(iseg >= seg_alloc)
			{
				seg_alloc *= 2;
				segs = (segment_t *)REALLOC(segs, seg_alloc, segment_t);
			}
			segs[iseg].start = ilines;
			segment_type_t segtype = get_segment_type(current, delta);
			if(segtype == SEG_INVAL)
			{
				printf("%s invalid segment type\n", current);
				exit(1);
			}

			segs[iseg].segtype = segtype;

			iseg++;

		}
		//strdup
		char *split =  strdup(current);

		//save it
		if(ilines >= sl_alloc)
			splitlines = REALLOC(splitlines, sl_alloc, char *);

		splitlines[ilines] = split;
		//printf("%s\n", splitlines[ilines]);
		last = *line;

	}
	if(iseg == 0)
	{
		printf("no segments at all\n");
		exit(1);
	}
	segs[iseg].range = ilines - segs[iseg - 1].start;
	size_t nlines = ilines;
	size_t nrefs = iref;
	for(int i = 0; i < nrefs; ++i)
	{
		//printf("%s\n", refs[i].str);
		//printf("%s\n", splitlines[refs[i].line]);
	}
	free(freferences);
	free(flines);
	free(fsegments);

	context->segments = segs;
	context->segments_len = iseg;
	context->splitlines = splitlines;
	context->refs = refs;
	context->splitlines_len = nlines;
	context->refs_len = nrefs;
}

const int tabsize = 4;

int get_following_indented(char **lines, int length, int index)
{
	int following = 0;
	for(int i = 0; i < length; ++i)
	{
		int i = get_starting_tabs_count(lines[i], tabsize);
		if(i <= 0)
		{
			break;
		}
		else
		{
			following++;
		}
	}
	return following;
}


void print_reference(reference_t *ref)
{
	printf("reference: '%s' : %d at address: [%lu] type: %d\n",ref->str,ref->line,ref->address, ref->rtype);
}

void free_reference(void *ref)
{
	reference_t *ptr = (reference_t *)ref;
	free(ptr->str);
	free(ptr);
}

reference_type_t reference_from_segment(segment_t *segment)
{
	//printf("segment type: %d\n", segment->segtype);
	switch(segment->segtype)
	{
		case SEG_DATA:
			return REF_DATA;
		case SEG_TEXT:
			return REF_TEXT;
		case SEG_REALLOC:
			return REF_REALLOC;
		default:
			return REF_INVAL;
	}
}

codeline_t enscribe_line(context_t *ctx, char *line, codeline_t last)
{
	int mode = 0;


}


asmlines_t determine_lines(context_t *ctx)
{
	asmlines_t al;
	int count = 0;
	size_t current_refs = 0;
	size_t segs_iter = 0;
	codeline_t *codeline = (codeline_t *)CALLOC(ctx->splitlines_len, codeline_t);

	p_hashtable_t table = new_hash_table(ctx->refs_len, free_reference);

	for(int entry = 0; entry < ctx->refs_len; entry++)
	{
		printf("refs[%d].address = %s\n", entry, ctx->refs[entry].str);

		addto_hash_table(table, ctx->refs[entry].str, &(ctx->refs[entry]));
	}

	segment_t *segment_current = NULL;

	int mode = 0;

	for(int i = 0; i < ctx->splitlines_len; ++i)
	{
		//print_hash_table(table);

		if(ctx->splitlines[i] == NULL)

			continue;
		if(ctx->splitlines[i][0] == 0)
			codeline[i] = CL_COMMENT;
		else if(ctx->segments[segs_iter].start == i)
		{
			codeline[i] = CL_SEGMENT;
			segment_current = &(ctx->segments[segs_iter]);
			//printf("segment used %d\n", ctx->segments[segs_iter].segtype);
			segs_iter++;
		}
		else if(ctx->refs[current_refs].line == i)
		{

			codeline[i] = CL_REFERENCE;
			ctx->refs[current_refs].address = count;
			ctx->refs[current_refs].segment_id = segs_iter;
			if(segment_current == NULL)
			{
				printf("current segment is NULL line: %d", i);
				exit(1);
			}
			ctx->refs[current_refs].rtype = reference_from_segment(segment_current);
			current_refs++;

		}
		else if(ctx->splitlines[i][0] == '\t' )
		{

			codeline[i] = CL_INSTRUCTION;
			count++;
			//printf("codeline %d\n", i);
			//printf("%x: %s\n",i , ctx->splitlines[i]);
		}
		else if(ctx->splitlines[i][0] == '#')
		{
			codeline[i] = CL_COMMENT;
		}

		else
		{
			codeline[i] = CL_INVALID;
		}
	}
	ctx->lines.ref_table = table;

	al.ref_table = table;
	al.lines_ary = codeline;
	al.ref_size = current_refs;
	al.lines_size = count;
	return al;
}


void init_asm_decode(context_t *ctx)
{
	asmlines_t al = determine_lines(ctx);
	ctx->lines = al;



	codeline_t *codeline = al.lines_ary;

	const int icount = al.lines_size;

	ctx->decode.table_bin_len = (icount / CODE_DESC_STRIDE) + 1;
	ctx->decode.instr_ary = CALLOC(icount, inst_t);
	ctx->decode.imm_ary = CALLOC(icount, uint64_t);

	ctx->decode.table_offsets_ary = CALLOC(ctx->decode.table_bin_len, uint64_t);
	//reusing this
	//immediate index
	ctx->decode.imm_iter = 0;

	//instruction index
	ctx->decode.instr_iter = 0;
	//code description count
	ctx->decode.table_offsets_iter = 0;
}

void instruction_pull(context_t *ctx, int index)
{

	asmdecode_t *dec = &ctx->decode;
	if(dec->instr_iter % CODE_DESC_STRIDE == 0)
	{

		dec->table_offsets_ary[dec->table_offsets_iter++] = dec->imm_iter;
		//printf("immoffsets[%d]=%llu\n",dec->table_offsets_iter - 1 ,dec->table_offsets_ary[dec->table_offsets_iter-1]);
	}

	inst_t temp = {};//create_instruction(ctx->splitlines[index], index);
	if(temp.immflag || temp.immref)
	{
		if(temp.immref != NULL)
		{
			//printf("immref: %s\n", temp.immref);
			reference_t *ref = (reference_t *)getdata_from_hash_table(ctx->lines.ref_table, temp.immref);
			if(ref == NULL)
			{
				printf("ref key '%s' not found in table\n", temp.immref);
				print_hash_table(ctx->lines.ref_table);
				exit(1);
			}
			if(ref->rtype == REF_REALLOC)
			{
				temp.aux = 1;
			}
			if(ref->rtype == REF_INVAL)
			{
				printf("reference is invalid %s\n", ref->str);
				exit(1);
			}


			temp.imm = ref->address;
			//printf("address = %llu\n", ref->address);

		}

		set_binary_immedates(ctx->binary, dec->imm_iter++, temp.imm);
	}
	dec->instr_ary[dec->imm_iter] = temp;
			//printf("instruction is: at: %.4x %.8x: ", iind, encode_inst(&temp));
	//print_inst(&temp);
	set_binary_instruction(ctx->binary, dec->instr_iter, encode_inst(&temp));

			//printf("imm: %d %llu\n", immind, temp.imm);

			//printf("trying\n");
	dec->instr_iter++;
}




entry_t *new_entry(context_t *ctx, reference_t *ref, uint64_t ln_start, uint64_t ln_end)
{

	entry_t *ent = CALLOC(1, entry_t);

	ent->line_number = ln_start;
	ent->ref = ref;
	ent->type;
	for(int i = ln_start; i < ctx->splitlines_len; ++i)
	{

	}

}


uint64_t second_stage(context_t *context)
{

	init_asm_decode(context);

	for(int i = 0; i < context->refs_len; ++i)
	{
		print_reference(&context->refs[i]);
	}
	segment_t *cseg = NULL;
	int segment_iter = 0;
	for(int x = 0; x < context->splitlines_len; ++x)
	{
		if(context->lines.lines_ary[x] != CL_SEGMENT && cseg == NULL)
		{
			printf("segment not defined at line:%d\n", x);
		}
		else if(context->lines.lines_ary[x] == CL_SEGMENT)
		{
			cseg = &context->segments[segment_iter];
			segment_iter++;

		}

		if(context->lines.lines_ary[x] == CL_REFERENCE)
		{
			switch (cseg->segtype)
			{
				case SEG_TEXT:
					break;
				case SEG_DATA:
					break;
				case SEG_REALLOC:
					break;
				default:
					break;
			}
		}
		//printf("%d\n", x);
		if(context->lines.lines_ary[x] == CL_INSTRUCTION)
		{
			instruction_pull(context, x);
		}


	}

	context->binary->table = context->decode.table_offsets_ary;
	context->binary->table_len = context->decode.table_offsets_iter;

	int table_total_len = context->decode.table_offsets_len;
	int code_size = context->decode.imm_len + context->decode.instr_len;
	int total_size = code_size + table_total_len + 6;
	return total_size;
}


void final_stage(asmbinary_t *binary, int length, char *file_name)
{

	uint64_t *bin = (uint64_t *)CALLOC(length, uint64_t);


	const uint64_t ct_actual_len = binary->table_len;
	//table
	int offset = 0;
	bin[offset = 0] = 6;
	bin[offset = 1] = ct_actual_len;
	//code table actual length
	//inst ptr
	const uint64_t instr_ptr = (binary->table_len) + 6;
	const uint64_t instr_len = binary->instructions_len;
	bin[offset = 2] = instr_ptr;
	bin[offset = 3] = instr_len;
	//imm ptr
	const uint64_t imm_ptr = (binary->table_len)+ binary->instructions_len + 6;
	const uint64_t imm_len = binary->immedates_len;
	bin[offset = 4] = imm_ptr;
	bin[offset = 5] = imm_len;
	//just buffering by 1
	offset = offset + 1;
	for(int table = 0; table < binary->table_len; table++, offset++)
	{
		//printf("table[%d]=%llu\n",table, binary->table[table]);
		bin[offset] = binary->table[table];
	}
	for(int c = 0; c < binary->instructions_len; ++c)
	{
		//inst_t i = decode_inst(context->instructions[c]);
		//print_inst(&i);
		bin[offset++] = binary->instructions[c];
	}
	for(int i = 0; i <= binary->immedates_len; ++i)
	{
		bin[offset++] = binary->immedates[i];
	}
	if(offset <= length)
	{
		//fuck
		ERR(ERANGE, "offset is off");
		exit(1);
	}
	FILE *fp = fopen(file_name, "wb");
	if(fp == NULL)
	{
		ERR(EBADF, "could not load file");
		exit(1);
	}
	for(int i = 0; i < offset * sizeof(int64_t); i++)
	{
		if(i % sizeof(int64_t) == 0)
		{

			printf("%.3d: %.8llx %llu\n", i/8, bin[i / 8], bin[i / 8]);
		}

	}
	fwrite(bin, sizeof(uint64_t), offset, fp);
	fclose(fp);
}

asmbinary_t *init_binary_buffer(size_t alloc_init)
{
	asmbinary_t *binary = (asmbinary_t *)CALLOC(alloc_init, asmbinary_t);
	binary->immedates = CALLOC(alloc_init, uint64_t);
	binary->instructions = CALLOC(alloc_init, uint32_t);
	binary->immedates_alloc = alloc_init;
	binary->instructions_alloc = alloc_init;

	return binary;
}

void assemble(char *content, size_t length, char *file_name)
{

	static context_t context = {0};
	const size_t alloc_init = 100;
	asmbinary_t *binary = init_binary_buffer(alloc_init);
	context.binary = binary;
	first_stage(&context, content, length);
	uint64_t size = second_stage(&context);
	final_stage(context.binary, size, file_name);
}
