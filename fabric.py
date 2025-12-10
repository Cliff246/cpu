


def regname(name: str):

	if name.startswith("r"):
		i = int(name[1:])
		if( i >= 0 and i < 8):
			return i

	return False
class Instruction:

	def __init__(self, operation: str):

		self.operation: str = operation

		split = self.operation.split()
		if(len(split) != 4):
			raise ValueError("op code string is not valid " + self.operation)

		self.opcode: str = split[0]
		self.dst : str = split[1]
		self.src0: str = split[2]
		self.src1: str = split[3]





	def isimmedate(self):
		if(len(self.get_sources()) == 1 ):
			return True
		return False

	def get_sources(self):

		src1 = regname(self.src0)
		src2 = regname(self.src1)
		if(src1 == False):
			raise ValueError("src1 did not meet criteria")
		if(src2 == False):
			return [src1]
		return [src1, src2]

	def print_inst(self):
		print(self.operation)

class LedgerEntry:


	def __init__(self, row, col):
		self.row = row
		self.col = col
		self.fabric = 0
		self.consumed = False

	def __str__(self):
		return str(self.consumed)

	def __repr__(self):
		return "%d %d" % (self.consumed, self.fabric)

class Ledger:


	def __init__(self, rows, cols):
		self.matrix = []
		self.height = [0 for _ in range(cols)]
		self.lowest_row = 0
		self.rows = rows
		self.cols = cols
		for r in range(rows):
			row = []

			for c in range(cols):
				row.append(LedgerEntry(r, c))
			self.matrix.append(row)


	def get_highest_row(self, inst: Instruction):

		srcs = inst.get_sources()
		highest = 0

		for src in srcs:
			if self.height[src] > highest:
				highest = self.height[src]
		return highest


	def add_srcs_to_row(self, inst: Instruction):
		row = self.get_highest_row(inst)
		for src in inst.get_sources():
			self.matrix[row][src].consumed = True
			self.height[src] = row

	def add_dest_to_row(self, inst: Instruction, fabric_index: int):
		src_row = self.get_highest_row(inst)
		dst = regname(inst.dst)
		if dst is False:
			raise ValueError("dst did not meet criteria")

		dst_row = src_row + 1
		if dst_row >= self.rows:
			raise ValueError("ledger overflow")

		entry = self.matrix[dst_row][dst]
		entry.fabric = fabric_index
		entry.consumed = False
		self.height[dst] = dst_row

	def print_matrix(self):
		line = 0
		for r in self.matrix:

			print(line, r)
			line = line + 1



class Fabric:
	FABRIC_QUEUE_SIZE = 5
	def __init__(self, fabric_index):
		self.fabric_index = fabric_index

		self.fabric_queue: list[str]

	def inject_instruction(self, instruction):

		self.fabric_queue.append(instruction)




class CPU:
	REGCOUNT = 8
	MEMSIZE = 1000
	FABRICS_SIZE = 2
	EPOCHS = 50


	def __init__(self, program: list[str]):

		self.regs: list[str] = [0 for _ in range(self.REGCOUNT)]
		self.pc = 0
		self.program: list[Instruction] = [Instruction(x) for x in program]
		self.memory: list[int] = [0 for _ in range(self.MEMSIZE)]

		self.fabrics: list[Fabric] = [Fabric(x) for x in range(self.FABRICS_SIZE)]
		self.ledger = Ledger(self.EPOCHS, self.REGCOUNT)




	def step(self):
		inst: Instruction = self.program[self.pc]

		# Mark source epochs
		self.ledger.add_srcs_to_row(inst)

		# Choose a simple fabric (round‑robin)
		fabric_index = self.pc % self.FABRICS_SIZE

		# Mark destination epoch
		self.ledger.add_dest_to_row(inst, fabric_index)

		self.pc += 1





	def print_program(self):
		for x in self.program:
			x.print_inst()

program = [
	"add r0 r1 r2",
	"addi r1 r2 100",
	"sub r2 r1 r1",
	"sub r5 r3 r0"
]


cpu = CPU(program)
cpu.print_program()

while(cpu.pc < len(cpu.program)):
	cpu.step()

cpu.ledger.print_matrix()