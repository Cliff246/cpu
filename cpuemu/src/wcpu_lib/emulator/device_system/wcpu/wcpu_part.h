#ifndef __WCPU_PART_HEADER__
#define __WCPU_PART_HEADER__

#include "wcpu_part_ptr.h"

#include "fetcher.h"
#include "lsu.h"
#include "register_file.h"
#include "mattress.h"
#include "aggregator.h"
#include "cache.h"
#include "ledger.h"
#include "mattress.h"
#include "fabric.h"
#include "alu.h"



typedef union wcpu_part_ptr part_ptr_t;


#include "wcpu_isa.h"

#define WCPU_PART_LIST(PART) 	\
	PART(LSU)				  	\
	PART(MATTRESS)			  	\
	PART(REGFILE)	  			\
	PART(FETCHER)				\
	PART(CACHE)			 		\
	PART(AGGREGATOR)			\
	PART(LEDGER)				\
	PART(FABRIC)				\
	PART(ALU)					\


#define NAME_PART(n) WCPU_PART_ ## n
#define PART(n) NAME_PART(n) ,


typedef enum wcpu_part_type
{
	WCPU_PART_LIST(PART)
}part_type_t;

#define PART_COUNT(n) + 1
#define UNIQUE_PARTS (0 WCPU_PART_LIST(PART_COUNT))




typedef struct wcpu_part
{
	part_type_t type;
	part_ptr_t ptr;
}part_t;

typedef void (*part_step)( part_ptr_t part);
typedef part_ptr_t (*part_init)(void);



typedef struct wcpu_part_class
{
	part_step step;
	part_init init;


}wcpu_part_class_t;


part_t *wcpu_part_generate(part_type_t type);

void wcpu_part_step(part_t *part);


extern wcpu_part_class_t part_vtable[UNIQUE_PARTS];

#endif