from .flags import Flags
from .component import Component

"""
Arithmetic
	•	ADD → s1 + s2 + s3
	•	SUB → s1 - s2 - s3

Logic
	•	AND → s1 & s2 (if immflag, s1 & (s2 | s3))
	•	OR → s1 | s2 (if immflag, s1 | (s2 & s3))
	•	XOR → s1 ^ s2 (if immflag, s1 ^ (s2 ^ s3))
	•	NOT → ~s1

Shifts
	•	SLL → (s1 << (s3 & 0x3F))
	•	SRL → (s1 >> (s3 & 0x3F)) (logical)
	•	SRA → (s1 >> (s3 & 0x3F)) with sign-extension

Multiplication
	•	MUL → s1 * s2 (+ s3 if immflag)
	•	MULH → high 64 bits of signed × signed
	•	MULHU → high 64 bits of unsigned × unsigned
	•	MULHSU → high 64 bits of signed × unsigned

Division
	•	DIV → s1 / s2 (0 if div-by-zero; add s3 if immflag)
	•	DIVU → unsigned version
	•	REM → s1 % s2 (+ s3 if immflag)
	•	REMU → unsigned version

Comparisons
	•	SLT / SLE → signed less-than / less-equal
	•	SLTU → unsigned less-than
	•	SEQ / SNE → equal / not equal

⸻

Notes
	•	s3 is always present (no “two operand” variant).
	•	immflag is a routing hint, not an ALU operation — the decode/mux system decides whether s3 is immediate or register.
	•	Post-add (res += s3) only applies on MUL/DIV/REM family.
	•	NOP = ADD x0, x0, x0, immf=0.
"""


class ALU(Component):

	def __init__(self, component_type):
		super().__init__(component_type)

	def __call__(self, ctrlflags, s1, s2, s3, immflag):
		print("ALU: rs1=%d rs2=%d imm=%d " % (s1, s2, s3))
		match(ctrlflags):
			case Flags.ALU_ADD:
					return s1 + s2 + s3
			case Flags.ALU_SUB:
					return s1 - s2 - s3
			case Flags.ALU_AND:
				if immflag:
					return s1 & (s2 | s3)
				else:
					return s1 & s2
			case Flags.ALU_OR:
				if immflag:
					return s1 | (s2 & s3)
				else:
					return s1 | s2
			case Flags.ALU_XOR:
				if immflag:
					return s1 ^ (s2 ^ s3)
				else:
					return s1 ^ s2
			#applies on s3
			case Flags.ALU_SLL:
				#left logical
				shift = s3 & 0x3F
				return (s1 << shift) & ((1 << 64) - 1)
			case Flags.ALU_SRL:
				shift = (s3 & 0x3F)
				return (s1 % (1 << 64)) >> shift
			case Flags.ALU_SRA:
				shift = (s3 & 0x3F)
			# arithmetic right: sign-extend 64-bit a
				if s1 & (1 << 63):
					return ((s1 | (~((1 << 64) - 1))) >> shift) & ((1 << 64) - 1)
				else:
					return (s1 >> shift) & ((1 << 64) - 1)
			case Flags.ALU_DIV:

				if(s2 == 0):
					return 0
				else:
					res = s1 // s2
					if immflag:
						res += s3
					return res

			case Flags.ALU_DIVU:

				if(s2 == 0):
					return 0
				else:
					us1 = s1 & ((1 << 64) - 1)
					us2 = s2 & ((1 << 64) - 1)
					res = us1 // us2
					if immflag:
						res += s3
					return res
			case Flags.ALU_MUL:
				res = s1 * s2
				if immflag:
					res += s3
				return res
			case Flags.ALU_MULH:  # signed × signed, high 64
				prod = (s1 * s2) & ((1 << 128) - 1)
				return (prod >> 64) & ((1 << 64) - 1)

			case Flags.ALU_MULHU:  # unsigned × unsigned, high 64
				us1 = s1 & ((1 << 64) - 1)
				us2 = s2 & ((1 << 64) - 1)
				prod = us1 * us2
				return (prod >> 64) & ((1 << 64) - 1)
			case Flags.ALU_MULHSU:  # signed × unsigned, high 64
				us2 = s2 & ((1 << 64) - 1)
				prod = s1 * us2
				return (prod >> 64) & ((1 << 64) - 1)
			case Flags.ALU_REM:
				res = s1 % s2
				if immflag:
					res += s3
				return res
			case Flags.ALU_REMUL:
				us1 = s1 & ((1 << 64) - 1)
				us2 = s2 & ((1 << 64) - 1)
				res = us1 % us2
				if immflag:
					res += s3
				return res

			case Flags.ALU_NOT:
				# Bitwise NOT on s1, mask to 64 bits
				return (~s1) & ((1 << 64) - 1)
			case Flags.ALU_SLT:
				# Signed less than: s1 < s2
				def to_signed(val):
					return val if val < (1 << 63) else val - (1 << 64)
				return 1 if to_signed(s1 & ((1 << 64) - 1)) < to_signed(s2 & ((1 << 64) - 1)) else 0
			case Flags.ALU_SLE:
				# Signed less than or equal: s1 <= s2
				def to_signed(val):
					return val if val < (1 << 63) else val - (1 << 64)
				return 1 if to_signed(s1 & ((1 << 64) - 1)) <= to_signed(s2 & ((1 << 64) - 1)) else 0
			case Flags.ALU_SLTU:
				# Unsigned less than: s1 < s2
				us1 = s1 & ((1 << 64) - 1)
				us2 = s2 & ((1 << 64) - 1)
				return 1 if us1 < us2 else 0
			case Flags.ALU_SEQ:
				# Set if equal
				return 1 if (s1 & ((1 << 64) - 1)) == (s2 & ((1 << 64) - 1)) else 0
			case Flags.ALU_SNE:
				# Set if not equal
				return 1 if (s1 & ((1 << 64) - 1)) != (s2 & ((1 << 64) - 1)) else 0


			case _:
				raise ValueError("ALU FAILED")


