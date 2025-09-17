

from enum import Enum


class Flags:
	PATH_ALU = 0x0
	PATH_MEM = 0x1
	PATH_BR  = 0x2

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

	# BRANCH subops
	BR_BEQ   = 0x20
	BR_BNE   = 0x21
	BR_JMP   = 0x22  # jump to rs1 (imm ignored)

	# SYS (system/control) subops
	PATH_SYS = 0x3

	SYS_PREFETCH_OFF = 0x30  # speculation barrier for cache prefetch
	SYS_PREFETCH_ON  = 0x31  # re-enable prefetch
	SYS_FENCE		= 0x32  # drain LPQ/frames before continuing (ordering)
	SYS_GET_TIMER = 0x33
	SYS_GET_RETIRED = 0x34
	SYS_SET_JUMP = 0x35
	SYS_GET_MODE = 0x36


	# --- I/Imm cache-line metadata (mask-based, no decode on hot path) ---
	IC_LINE_SIZE = 4  # number of 32-bit ops per I-cache line

