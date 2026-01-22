#ifndef __WCC_MACROS_HEADER__
#define __WCC_MACROS_HEADER__

#define WCC_MEMORY_HEAP_SIZE (1024 * 1024)


#define WCC_TOKEN_TYPE_LIST(X)	\
	X(NONE)				  		\
	X(WORD)					  	\
	X(INT)						\
	X(STR)				  		\
	X(CHAR)				  		\
	X(HEX)				  		\
	X(FLOAT)					\
	X(COMMA)					\


#define WCC_TOKEN_TYPE_NAME(X)	TOK_TYPE_ ## X
#define WCC_TOKEN_TYPE_ENUM(X)  WCC_TOKEN_TYPE_NAME(X),

#define WCC_TOKEN_TYPE_ARYELEM(X) [WCC_TOKEN_TYPE_NAME(X)] = #X
#define WCC_TOKEN_TYPE_ARRAY(X) WCC_TOKEN_TYPE_ARYELEM(X),


#endif