#ifndef __TEST_ASSERT__
#define __TEST_ASSERT__

#include <stdint.h>
#include <stdlib.h>

typedef enum tassert_type
{
	ASSERT_INVAL,
	ASSERT_MEMORY,
	ASSERT_REGISTER,
}tassert_type_t;

typedef struct tassert_register
{
	int reg;
	int64_t expects;
}tassert_register_t;

typedef struct tassert_memory
{
	int start, stop;
	int64_t *expects;
	size_t count;
}tassert_memory_t;

typedef struct tassert
{
	tassert_type_t type;

	union
	{
		tassert_memory_t tamem;
		tassert_register_t tareg;
	}asrt;

}tassert_t;


tassert_t *create_tassert();


#endif