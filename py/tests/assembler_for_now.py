from dataclasses import dataclass

class instruction:

	path: int
	subop: int
	rd: int
	rs1: int
	rs2: int
	aux: int
	immflag: int

	immedate: int

	def __init__(self, path, subop, rd, rs1, rs2, aux, immflag = 0, imm = 0):
		self.path = path
		self.subop = subop
		self.rd = rd
		self.rs1 = rs1
		self.rs2 = rs2
		self.aux = aux
		self.immflag = immflag
		self.imm = imm if immflag == 1 else 0

	def enc(self):

		return ((self.path & 0xF)<<28) | ((self.subop & 0xFF)<<20) | ((self.rd & 0x1F)<<15) | ((self.rs1 & 0x1F)<<10) | ((self.rs2 & 0x1F)<<5) | ((self.aux & 0xF)<<1) | (self.immflag & 1)

	def get_as_tuple(self):
		return (self.path, self.subop, self.rd, self.rs1, self.rs2, self.aux, self.immflag)

	def has_immedate(self):
		return True if self.immflag == 1 else False



def _assemble(code: list[instruction]):
	binary = []

	# The assembler now expects a list of 'instruction' objects directly,
	# and handles encoding internally.

	# Lists to hold encoded instructions and immediates
	instructions = []
	immedates = []
	imm_counter = 0

	# Encode instructions and collect immediates
	instruction_table_set = []
	checkpoint_interval = 128
	pc = 0
	for instr in code:
		instructions.append(instr.enc())
		if instr.has_immedate():
			immedates.append(instr.imm)
			imm_counter += 1

		if pc % checkpoint_interval == 0:
			instruction_table_set.append(pc)
			instruction_table_set.append(imm_counter)
		pc = pc + 1
	# Build the immediate checkpoint table (instruction_table_set)
	# This table is used by the CPU loader for quick jumps to immediates



	instruction_table_set_length = len(instruction_table_set)
	inst_length = len(instructions)
	imm_length = len(immedates)

	# Compose the final binary layout
	# [instruction_table_set][instructions][immediates]
	binary.extend(instruction_table_set)
	binary.extend(instructions)
	binary.extend(immedates)

	instruction_table_set_base = 6
	inst_base = instruction_table_set_base + instruction_table_set_length
	imm_base = inst_base + inst_length

	# Fill the start table (header) at the very beginning of the binary
	# This header contains 6 entries used by the CPU loader:
	# [instruction_table_set_base, instruction_table_set_length, inst_base, inst_length, imm_base, imm_length]
	header = [
		instruction_table_set_base,
		instruction_table_set_length,
		inst_base,
		inst_length,
		imm_base,
		imm_length
	]

	# Insert the header at the start of the binary
	binary = header + binary

	return binary