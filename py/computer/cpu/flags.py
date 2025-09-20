

from enum import Enum


class Flags:
	PATH_ALU = 0x0
	PATH_MEM = 0x1
	PATH_BR  = 0x2
	PATH_SYS = 0x3

	# ALU subops
	ALU_ADD  = 0x01
	ALU_SUB  = 0x02
	ALU_AND  = 0x03
	ALU_OR   = 0x04
	ALU_XOR  = 0x05
	ALU_SLL  = 0x06  # logical left
	ALU_SRL  = 0x07  # logical right
	ALU_SRA  = 0x08  # arithmetic right
	ALU_DIV  = 0x09
	ALU_MUL =  0x0A
	ALU_REM = 0x0B
	ALU_REMU = 0x0C
	ALU_DIVU = 0x0D
	ALU_MULHI = 0x0E
	ALU_MULU = 0x0F
	ALU_MULUS = 0x10
	ALU_NOT = 0x11
	ALU_SLT = 0x12
	ALU_SLE = 0x13
	ALU_SLTU = 0x14
	ALU_SEQ = 0x15
	ALU_SNE = 0x16
	# MEM subops
	MEM_LD   = 0x10
	MEM_ST   = 0x11
	MEM_GET_SP = 0x12
	MEM_SET_SP = 0x13
	MEM_PUSH = 0x14
	MEM_POP = 0x15
	MEM_SET_SFP = 0x14
	MEM_GET_SFP = 0x15


	# BRANCH subops
	BR_BEQ   = 0x20
	BR_BNE   = 0x21
	BR_JMP   = 0x22  # jump to rs1 (imm ignored)

	# SYS (system/control) subops

	SYS_PREFETCH_OFF = 0x30  # speculation barrier for cache prefetch
	SYS_PREFETCH_ON  = 0x31  # re-enable prefetch
	SYS_FENCE		= 0x32  # drain LPQ/frames before continuing (ordering)
	SYS_GET_TIMER = 0x33
	SYS_GET_RETIRED = 0x34
	SYS_SET_JUMP = 0x35
	SYS_GET_MODE = 0x36
	SYS_CALL = 0x37
	SYS_BREAK = 0x38
	SYS_FRAME_SET = 0x39


	# --- I/Imm cache-line metadata (mask-based, no decode on hot path) ---
	IC_LINE_SIZE = 4  # number of 32-bit ops per I-cache line

	@staticmethod
	def convert_to_string_path(path):
		match path:
			case Flags.PATH_ALU:
				return "ALU"
			case Flags.PATH_BR:
				return "BR"
			case Flags.PATH_MEM:
				return "MEM"
			case Flags.PATH_SYS:
				return "SYS"

	#yes this can be done better
	@staticmethod
	def string_alu_subops(subops):

		match subops:
			case Flags.ALU_ADD:
				return "ADD"
			case Flags.ALU_SUB:
				return "SUB"
			case Flags.ALU_AND:
				return "AND"
			case Flags.ALU_OR:
				return "OR"
			case Flags.ALU_XOR:
				return "XOR"
			case Flags.ALU_SLL:
				return "SHIFT LEFT LOGICAL"
			case Flags.ALU_SRL:
				return "SHIFT RIGHT LOGICAL"
			case Flags.ALU_SRA:
				return "SHIFT RIGHT ARITHMETIC"
			case Flags.ALU_DIV:
				return "DIVIDE"
			case Flags.ALU_MUL:
				return "MULTIPLY"
			case Flags.ALU_MULHI:
				return "MULTIPLY HIGH"
			case Flags.ALU_MULU:
				return "MULTIPLY UNSIGNED"
			case Flags.ALU_MULUS:
				return "MULTIPLY SIGNED UNSIGED"
			case Flags.ALU_NOT:
				return "NOT"
			case Flags.ALU_SLT:
				return "SIGNED LESS THAN"
			case Flags.ALU_SEQ:
				return "SIGNED LESS THAN EQUAL"
			case Flags.ALU_SLTU:
				return "LESS THAN UNSIGEND"
			case Flags.ALU_SEQ:
				return "SIGNED EQUAL"
			case Flags.ALU_SNE:
				return "SIGNED NOT EQUAL"
			case _:
				return "ALU INVALID"

	@staticmethod
	def string_mem_subops(subop):

		match subop:
			case Flags.MEM_LD:
				return "LOAD"
			case Flags.MEM_ST:
				return "STORE"
			case Flags.MEM_PUSH:
				return "PUSH"
			case Flags.MEM_POP:
				return "POP"
			case Flags.MEM_GET_SFP:
				return "GET SFP"
			case Flags.MEM_SET_SFP:
				return "SET SFP"
			case Flags.MEM_GET_SP:
				return "GET SP"
			case Flags.MEM_SET_SP:
				return "SET SP"

			case _:
				return "MEM INVALID"

	@staticmethod
	def string_br_subops(subop):
		match subop:
			case Flags.BR_BEQ:
				return "EQUAL"
			case Flags.BR_BNE:
				return "NOTEQUAL"
			case Flags.BR_JMP:
				return "JUMP"
			case _:
				return "BR INVALID"

	@staticmethod
	def string_sys_subops(subop):
		match subop:
			case Flags.SYS_FENCE:
				return "FENCE"
			case Flags.SYS_GET_MODE:
				return "GET MODE"
			case Flags.SYS_GET_RETIRED:
				return "GET RETIRED"
			case Flags.SYS_GET_TIMER:
				return "GET TIMER"
			case Flags.SYS_PREFETCH_OFF:
				return "PREFETCH OFF"
			case Flags.SYS_PREFETCH_ON:
				return "PREFETCH ON"
			case Flags.SYS_SET_JUMP:
				return "SET JUMP"

			case _:
				return "SYS INVALID"


	@staticmethod
	def convert_subop(path, subop):

		match  path:
			case Flags.PATH_ALU:
				return Flags.string_alu_subops(subop)
			case Flags.PATH_MEM:
				return Flags.string_mem_subops(subop)
			case Flags.PATH_BR:
				return Flags.string_br_subops(subop)
			case Flags.PATH_SYS:
				return Flags.string_sys_subops(subop)
			case _:
				return "PATH INVALID"