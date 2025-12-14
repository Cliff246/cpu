#ifndef __WCPU_FABRIC_PART_PTR_HEADER__
#define __WCPU_FABRIC_PART_PTR_HEADER__


typedef struct wcpu_fabric_alu fabric_alu_t;
typedef struct wcpu_fabric_bus fabric_bus_t;
typedef struct wcpu_fabric_controller fabric_controller_t;
typedef struct wcpu_fabric_executer fabric_exectuer_t;
typedef struct wcpu_fabric_regfile fabric_regfile_t;


typedef union wcpu_fabric_part_ptr
{
	fabric_alu_t *alu;
	fabric_bus_t *bus;
	fabric_controller_t *controller;
	fabric_exectuer_t *executer;
	fabric_regfile_t *regfile;
	void *raw;
}fabric_part_ptr_t;



//part types list should match the structs
#define FABRIC_PART_TYPES_LIST(X)			\
	X(FAB_ALU)						\
	X(REGFILE)						\
	X(CONTROLER)					\
	X(BUS)							\
	X(EXECUTER)						\



#define FABRIC_PART_NAME(C) FABRIC_PART_ ## C
#define FABRIC_PART_TYPE_ENUM(x) FABRIC_PART_NAME(x),
typedef enum fabric_part_type
{
	FABRIC_PART_TYPES_LIST(FABRIC_PART_TYPE_ENUM)
}fabric_part_type_t;


#define FABRIC_PART_TYPE_ADD(x) + 1

#define FABRIC_PARTS_LENGTH (0 FABRIC_PART_TYPES_LIST(FABRIC_PART_TYPE_ADD))


#endif