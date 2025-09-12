
from .flags import Flags
from .component import Component
class ALU(Component):

	def __init__(self, component_type):
		super().__init__(component_type)

	def __call__(self, *args, **kwds):
		ctrlflags, s1, s2, s3 = args

		match(ctrlflags):
			case Flags.ALU_ADD:
				return s1 + s2 + s3
			case Flags.ALU_ADDI:
				return s1 + s2 + s3
			case Flags.ALU_SUB:
				return s1 - s2 - s3
			case Flags.ALU_AND:
				return s1 & s2
			case Flags.ALU_OR:
				return s1 | s2
			case Flags.ALU_XOR:
				return s1 ^ s2 ^ s3
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
			case _:
				raise ValueError("ALU FAILED")
