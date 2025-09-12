

from enum import Enum


class Flags:
	PATH_ALU = 0x0
	PATH_MEM = 0x1
	PATH_BR  = 0x2

	# ALU subops
	ALU_ADD  = 0x01
	ALU_ADDI = 0x02
	ALU_SUB  = 0x03
	ALU_AND  = 0x04
	ALU_OR   = 0x05
	ALU_XOR  = 0x06
	ALU_SLL  = 0x07  # logical left
	ALU_SRL  = 0x08  # logical right
	ALU_SRA  = 0x09  # arithmetic right

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

	# --- I/Imm cache-line metadata (mask-based, no decode on hot path) ---
	IC_LINE_SIZE = 8  # number of 32-bit ops per I-cache line

