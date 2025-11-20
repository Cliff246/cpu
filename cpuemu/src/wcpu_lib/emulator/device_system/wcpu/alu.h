#ifndef _ALU_HEADER__
#define _ALU_HEADER__

#include "wcpu_part_ptr.h"

#include <stdint.h>
#include <stdbool.h>

#define LOGIC_UNIT_LIST(X) \
	X(ADDER		, 1)	  \
	X(SHIFTER   , 1)	  \
	X(OR		, 1) 	  \
	X(AND 		, 1)	  \
	X(MUL		, 4)	  \
	X(DIV		, 40)	  \
	X(REM		, 50)	  \

//Yada yaya
#define ADD_NAME(n) LOGIC_ ## n
#define X(a,b) ADD_NAME(a),



typedef enum logic_unit_type
{
	LOGIC_UNIT_LIST(X)
}logic_unit_type_t;


typedef struct logic_operation
{
	int64_t lane1, lane2, lane3;
	int64_t dest;
	int fabric_owner;
	char operation;

	char immf;

}logic_operation_t;


typedef enum logic_unit_state
{
	LOGIC_UNIT_INVAL,
	LOGIC_UNIT_EMPTY,
	LOGIC_UNIT_WORKING,
	LOGIC_UNIT_FAULT,
	LOGIC_UNIT_DONE,

}logic_unit_state_t;

typedef struct logic_unit
{
	//logic unit type
	logic_unit_type_t type;
	logic_unit_state_t state;
	int id;
	int cycles_completed;
	int cycles_needed;
	//todo change this to enum
	char fault;

}logic_unit_t;




//get logic unit state
logic_unit_state_t get_logic_unit_state(logic_unit_t *lu);


//returns if the logic unit is done
bool cycle_logic_unit(logic_unit_t *lu);

//gets the logic units result if done
int64_t get_logic_unit_result(logic_unit_t *lu);






//first stage logic units for an alu
#define MALU_LU_ORDER_LIST1(MALU_1X) \
	MALU_1X(ADDER)					\
	MALU_1X(OR)						\
	MALU_1X(AND)					\
	MALU_1X(XOR)					\
	MALU_1X(SHIFT_LEFT)				\
	MALU_1X(SHIFT_RIGHT)			\
//just a name
#define MALU_LU_ORDER_NAME(n,X) MALU_LU_ORDER ## n ## _ ## X


//apply to enum so we can look up faster
//ie active unit = [MALU_LU_ORDER1_ADDER] for an add instruction
//also allows us to 'poke' them


#define MALU_1X(x) MALU_LU_ORDER_NAME(1,x),

typedef enum micro_alu_lu_order1
{
	MALU_LU_ORDER_LIST1(MALU_1X)
}malu_lu_ord1_t;

#define MALU_LU_ORDER_LIST2(MALU_2X) \
	MALU_2X(ADDER)					 \

#define MALU_2X(x) MALU_LU_ORDER_NAME(2,x),


//order 2 enum list
typedef enum micro_alu_lu_order2
{
	MALU_LU_ORDER_LIST2(MALU_2X)
}malu_lu_ord2_t;

#define ADD_PLUS_1(X) + 1

#define MALU_1X_SIZE (0 + MALU_LU_ORDER_LIST1(ADD_PLUS_1))
#define MALU_2X_SIZE (0 + MALU_LU_ORDER_LIST2(ADD_PLUS_1))


//micro adder in a fabric
typedef struct micro_alu
{

	logic_unit_t stage1_units[MALU_1X_SIZE];
	logic_unit_t stage2_units[MALU_2X_SIZE];
	malu_lu_ord1_t stage1_active;
	malu_lu_ord2_t stage2_active;

}micro_alu_t;

#define GALU_LU_ORDER_LIST1(GALU_1X) \
	GALU_1X(ADDER)					\
	GALU_1X(OR)						\
	GALU_1X(AND)					\
	GALU_1X(XOR)					\
	GALU_1X(SHIFT_LEFT)				\
	GALU_1X(SHIFT_RIGHT)			\
	GALU_1X(MULTIPLY)				\
	GALU_1X(DIVIDE)					\
	GALU_1X(REMANDER)				\
	GALU_1X(RAND)					\
	GALU_1X(MIN)					\


#define GALU_LU_ORDER_NAME(n,X) GALU_LU_ORDER ## n ## _ ## X

#define GALU_1X(x) GALU_LU_ORDER_NAME(1,x),

typedef enum global_alu_lu_order1
{
	GALU_LU_ORDER_LIST1(GALU_1X)
}global_lu_ord1_t;


#define GALU_LU_ORDER_LIST2(GALU_2X) \
	GALU_2X(ADDER)					 \

#define GALU_2X(x) GALU_LU_ORDER_NAME(2,x),


typedef enum global_alu_lu_order2
{
	GALU_LU_ORDER_LIST2(GALU_2X)
}global_lu_ord2_t;


#define GALU_1X_SIZE (0 + GALU_LU_ORDER_LIST1(ADD_PLUS_1))
#define GALU_2X_SIZE (0 + GALU_LU_ORDER_LIST2(ADD_PLUS_1))


typedef struct global_alu
{

	logic_unit_t stage1_units[GALU_1X_SIZE];

	logic_unit_t stage2_units[GALU_2X_SIZE];
	global_lu_ord1_t stage1_unit;
	global_lu_ord2_t stage2_unit;


}global_alu_t;



typedef enum alu_type
{
	ALU_TYPE_UNKNOWN,
	ALU_TYPE_MICRO,
	ALU_TYPE_GLOBAL,
}alu_type_t;

typedef struct alu
{
	alu_type_t type;

	union
	{
		global_alu_t *global;
		micro_alu_t *micro;
	}unit;

}alu_t;



void set_alu_instructions(void);

part_ptr_t wcpu_alu_generate(void);

void alu_submit(alu_t *alu, char subpath, int64_t rs1, int64_t rs2, int64_t imm, char immflag);

void wcpu_alu_upadate(part_ptr_t alu);

int alu_poll(alu_t *alu);

#endif
