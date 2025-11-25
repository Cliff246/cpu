#ifndef __WCPU_PART_HEADER__
#define __WCPU_PART_HEADER__

#include "wcpu_part_ptr.h"

#include "fetcher.h"
#include "lsu.h"
#include "register_file.h"
#include "aggregator.h"
#include "cache.h"
#include "ledger.h"
#include "mattress.h"
#include "fabric.h"
#include "alu.h"



#include "wcpu_part_bus.h"

#include "wcpu_isa.h"

#define WCPU_PART_LIST(PART) 	\
	PART(LSU)				  	\
	PART(MATTRESS)			  	\
	PART(REGFILE)	  			\
	PART(FETCHER)				\
	PART(CACHE)			 		\
	PART(AGGREGATOR)			\
	PART(LEDGER)				\
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
	part_bus_t bus;
	int id;
}part_t;

typedef void (*part_step)( part_t *part);
typedef void (*part_import)( part_t *part, part_signal_t *signal);
typedef bool (*part_export)( part_t *part, part_signal_t **siganl);
typedef part_ptr_t (*part_init)(void);



typedef struct wcpu_part_class
{
	part_step step;
	part_init init;
	part_import import;
	part_export export;

}wcpu_part_class_t;


part_t *wcpu_part_generate(part_type_t type);

void wcpu_part_step(part_t *part);

part_signal_t part_bus_pop_signal(part_t *part);
void part_bus_push_signal(part_t *part, part_signal_t signal);

extern wcpu_part_class_t part_vtable[UNIQUE_PARTS];

#endif