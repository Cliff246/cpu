#ifndef __CODE_DECODER__
#define __CODE_DECODER__



#define PATH_STR_SIZE 8
#define REG_STR_SIZE 5


extern char path_codes[16][PATH_STR_SIZE];
extern char subpath_codes[16][0xff][8];

int path_to_string(char buffer[PATH_STR_SIZE], int code);

//char *operation_to_text_static_buffer(operation_t *op);
//char *convert_operation_to_text(operation_t *op);

#endif
