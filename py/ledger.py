
class Instruction:

	def __init__(self, opcode: str, owner):

		self.opcode: str = opcode

		parts = opcode.split(' ')
		self.op = parts[0]




		self.rd = int(parts[1][1:])
		self.rs1 = int(parts[2][1:])
		self.rs2 = int(parts[3][1:])
		self.owner = owner

	def __str__(self):
		return self.opcode


class Ledger:

	def __init__(self, deps):

		self.deps = deps
		self.current_epoch = 0
		self.content: list[list] = []

		new = [0 for _ in range(deps)]
		self.content.append(new)





	def mark(self, rd, rs1, rs2, owner):

		new_row = self.content[self.current_epoch].copy()

    	# 2. record the destination owner
		new_row[rd] = owner

 		# 3. save dependencies on the instruction instead of mutating ledger
 		# (rs1, rs2 are NOT written into ledger)
 		# instruction.deps = {rs1, rs2}
 		# 4. append new row and advance epoch
		self.content.append(new_row)
		self.current_epoch += 1


	def print_epoch(self, epoch):

		row = "%2d| " % epoch

		for i in self.content[epoch]:
			row += "%d  " % i
		print(row)

	def print_ledger(self):

		start_row = "    "
		for x in range(self.deps):
			start_row += "x%d " %x
		print(start_row)
		for i in range(len(self.content)):
			self.print_epoch(i)


insts = [
	Instruction("mul x0 x1 x2", 1),
	Instruction("add x1 x2 x3", 2),
	Instruction("sub x1 x1 x1", 2),
	Instruction("div x1 x1 x1", 1),
	Instruction("add x3 x1 x1", 1),
]





ledger = Ledger(4)

for i in insts:
	print(i)
	ledger.mark(i.rd, i.rs1, i.rs2, i.owner)
ledger.print_ledger()
