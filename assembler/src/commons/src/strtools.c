#include "strtools.h"
#include "commons.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char *clear_whitespace(char *str)
{
	int length = strlen(str);
	char temp[length + 1];
	memset(temp, 0, length + 1);
	int count = 0;
	for(char *ch = str; *ch; ch++)
	{
		if(*ch != ' ')
		{
			temp[count++] = *ch;
		}
	}
	return strdup(temp);

}

void clean_string(char *str)
{
#define MAX_CLEAN 1024
	int length = strlen(str);
	char temp[MAX_CLEAN + 1] = {0};
	if(length > MAX_CLEAN)
	{

		printf("do clean string better\n");
		exit(1);
	}
	memset(temp, 0, length);
	int count = 0;
	for(char *ch = str; *ch && count < length; ch++)
	{
		if(*ch != ' ' && *ch != '\n' || *ch != '\t')
		{
			temp[count++] = *ch;
		}
	}
	strncpy(str, temp, length);
}

int split_str(const char *string, char ***ptr, const char *delims)
{

	size_t slength = strlen(string);
	size_t dlength = strlen(delims);
	char *where = (char *)CALLOC(slength, char);
	memset(where, 0, slength * sizeof(char));
	int splits =0;

	bool  wasspliting = true, didsplit;
	int last = 0;
	for(int x = 0; x < slength; ++x)
	{

		//printf("x: %d\n", x);
		didsplit = false;
		for(int y = 0; y < dlength; ++y)
		{
			if(string[x] == delims[y])
			{
				didsplit = true;
				where[x] = string[x];
				continue;
			}
		}
		if(didsplit)
		{
			if(wasspliting == false)
			{
				wasspliting = true;
			}
		}
		else
		{
			//did not split
			if(wasspliting == true)
			{
				splits++;
				wasspliting = false;
			}
		}

	}

	//for(int i = 0; i < slength; ++i)
	{

		//printf("%c %d\n", string[i],where[i]);
	}
	char **splitary = CALLOC(splits + 1, char *);
	if(splitary == NULL)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}
	char *buffer = CALLOC(slength + 1, char);
	if(!buffer)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}
	int ibuf = 0;
	int last_good = 0;
	bool back_to_back = (where[0]>0)? true : false;
	int current = 0;
	for(int i = 0, cur = 0; i < slength; ++i, ++cur)
	{
		if(where[i] == 0)
		{
			if(back_to_back == true)
			{
				last_good = i;
				back_to_back = false;
			}
			buffer[ibuf++] = string[i];
		}
		else
		{
			if(!back_to_back)
			{
				int delta = (i - 1) - last_good;
				splitary[current++] = strdup(buffer);
				//printf("%s\n", splitary[current - 1]);
				ibuf = 0;
				memset(buffer, 0, slength + 1);
				back_to_back = true;
			}
		}
	}
	if (current != splits)
	{
    	fprintf(stderr, "Token count mismatch: predicted %d, actual %d\n", splits, current);
	}

	if(where[slength] == 0)
	{
		int delta = (slength - 1) - last_good;
		splitary[current++] = strdup(buffer);
		//printf("%s\n", splitary[current - 1]);
		ibuf = 0;
		memset(buffer, 0, slength + 1);
	}
	else
	{
		int delta = (slength - 2) - last_good;
		splitary[current++] = strdup(buffer);
		//printf("%s\n", splitary[current - 1]);
		ibuf = 0;
		memset(buffer, 0, slength + 1);

	}
	free(buffer);
	free(where);
	*ptr = splitary;
	return splits;
}

int determine_code(char *keyword, const char *const mnemonics[], int length)
{

	//printf("'%s'\n", keyword);
	int *keycount = CALLOC(length, int);
	if(keycount == 0)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}


	int i = 0;
	for(char *ch = keyword; *ch; ++ch, ++i)
	{

		for(int pool = 0; pool < length; ++pool)
		{
			//printf("op:%s n:%d\n", mnemonics[pool], keycount[pool]);
			if(keycount[pool] == -1)
			{
				continue;
			}
			if(mnemonics[pool][i] == '\0')
			{
				keycount[pool] = -1;
			}
			else if(mnemonics[pool][i] == *ch)
				keycount[pool] += 1;
			else
				keycount[pool] = -1;
		}
	}

	int max = 0, index = 0;
	for(int k = 0; k < length; ++k)
	{
		if(keycount[k] > max)
		{
			max = keycount[k];
			index = k;

		}
	}

	free(keycount);

	if(strlen(mnemonics[index]) == max)
	{
		return index;
	}
	return -1;
}



int get_starting_tabs_count(char *str, int tabsize)
{
	int count = 0;
	int spaces = 0;

	for(char* scroll = str; *str; scroll++)
	{
		if(*scroll = '\t')
		{
			count++;
		}
		else if(*scroll == ' ')
		{
			spaces++;
		}
		else
		{
			return count;
		}
		if(spaces == tabsize)
		{
			spaces = 0;
			count++;
		}
	}
	return count;
}


char basic_strings[256][2] = {
	0
};

char *to_string(char val)
{

	if(basic_strings[(unsigned char)val][0] != val)
	{
		basic_strings[val][0] = val;
	}
	return basic_strings[val];
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

	int index = 0;
	for(int i = 0; i < length - 1; ++i)
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
	if(hasref == true)
	{
		references[index++] = last_line;
	}

	if(index + 1 == alloc)
	{
		alloc += 10;
		references = REALLOC(references, alloc, int);
	}
	references[index] = -1;
	return references;
}

int *collect_segments(char *content, size_t length)
{

	size_t alloc = 100;
	int *segments = (int *)CALLOC(alloc, int);
	if(!segments)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}

	int last_line = 0;
	int index = 0;
	bool hasseg = false;
	bool newline = true;
	for(int i = 0; i < length; ++i)
	{
		if(content[i] == '\n')
		{
			newline = true;
			if(hasseg == true)
			{
				if(index + 1 == alloc)
				{
					alloc *= 2;
					segments = REALLOC(segments, alloc, int);
				}
				segments[index++] = last_line;
			}
			last_line = i;
			hasseg = false;
		}
		else if(newline == true && content[i] == '.')
		{
			hasseg = true;
			newline = false;
		}
		else
		{
			newline = false;
		}
	}
	if(hasseg == true)
	{
		segments[index++] = last_line;

	}

	if(index + 1 == alloc)
	{
		alloc += 10;
		segments = REALLOC(segments, alloc, int);
	}
	segments[index] = -1;
	return segments;
}


void int64_to_8chars(int64_t num, char dest[8])
{
	int64_t cur = num;
	for(int i = 0; i < 8; ++i)
	{
		dest[i] = cur & 0xff;
		cur >>= 8;
	}
}