from computer import *



def test1():

	def encode(path, subop, rd, rs1, rs2, aux=0, immflag=0):
		"""Pack an instruction into your 32-bit layout"""
		return ((path & 0xF) << 28) | ((subop & 0xFF) << 20) | ((rd & 0x1F) << 15) \
			   | ((rs1 & 0x1F) << 10) | ((rs2 & 0x1F) << 5) | ((aux & 0xF) << 1) \
			   | (immflag & 0x1)

	# --- program ---
	instrs = []
	imms   = []

	# r1 = MEM[100]
	instrs.append(encode(Flags.PATH_MEM, Flags.MEM_LD, rd=1, rs1=0, rs2=0, immflag=1))
	imms.append(100)

	# r2 = MEM[101]
	instrs.append(encode(Flags.PATH_MEM, Flags.MEM_LD, rd=2, rs1=0, rs2=0, immflag=1))
	imms.append(101)

	# r3 = r1 + r2
	instrs.append(encode(Flags.PATH_ALU, Flags.ALU_ADD, rd=3, rs1=1, rs2=2))

	# MEM[102] = r3
	instrs.append(encode(Flags.PATH_MEM, Flags.MEM_ST, rd=0, rs1=0, rs2=3, immflag=1))
	imms.append(102)

	# --- run it ---
	cpu = WeirdoCPU()
	cpu.load_program(instrs, imms)

	# preload memory
	cpu.MEM[100] = 7
	cpu.MEM[101] = 5

	while cpu.INSTRUCT_PC < len(cpu.INSTR):
		cpu.step()

	print("MEM[102] =", cpu.MEM[102])  # expect 12
	assert(cpu.MEM[102] == 12)



def test2():
	def enc(path, subop, rd, rs1, rs2, aux=0, immflag=0):
		return ((path & 0xF)<<28) | ((subop & 0xFF)<<20) | ((rd & 0x1F)<<15) \
			   | ((rs1 & 0x1F)<<10) | ((rs2 & 0x1F)<<5) | ((aux & 0xF)<<1) | (immflag & 1)

	# registers
	r0=0; r1=1; r2=2; r3=3; r4=4; r5=5; r6=6

	instrs=[]; imms=[]

	def emit(i, imm=None):
		instrs.append(i)
		if imm is not None: imms.append(imm)

	# r2 = base (100)
	emit(enc(Flags.PATH_ALU, Flags.ALU_ADD, r2, r0, 0, immflag=1), 100)
	# r3 = count (5)
	emit(enc(Flags.PATH_ALU, Flags.ALU_ADD, r3, r0, 0, immflag=1), 5)
	# r4 = 0 (accum)
	emit(enc(Flags.PATH_ALU, Flags.ALU_ADD, r4, r0, 0, immflag=1), 0)

	loop = len(instrs)

	# LD r5, [r2 + 0]
	emit(enc(Flags.PATH_MEM, Flags.MEM_LD, r5, r2, 0, immflag=1), 0)
	# r4 += r5
	emit(enc(Flags.PATH_ALU, Flags.ALU_ADD, r4, r4, r5))
	# r2 += 1 (next element)
	emit(enc(Flags.PATH_ALU, Flags.ALU_ADD, r2, r2, 0, immflag=1), 1)
	# r3 += -1
	emit(enc(Flags.PATH_ALU, Flags.ALU_ADD, r3, r3, 0, immflag=1), -1)
	# if r3 != 0, branch back to loop
	branch_pc = len(instrs)# the index this branch will sit at
	back = loop - branch_pc
	emit(enc(Flags.PATH_BR, Flags.BR_BNE, 0, r3, r0, immflag=1), back)

	# store sum at MEM[200]
	emit(enc(Flags.PATH_MEM, Flags.MEM_ST, 0, r6, r4, immflag=1), 0)

	cpu = WeirdoCPU()
	cpu.load_program(instrs, imms)

	# data: MEM[100..104] = 3,1,4,1,5 ; r6 = 200
	cpu.REGS[r6] = 200
	base=100
	for i,v in enumerate([3,1,4,1,5]):
		cpu.MEM[base+i] = v

	# run
	while cpu.INSTRUCT_PC < len(cpu.INSTR):
		cpu.step()

	print("sum -> MEM[200] =", cpu.MEM[200])  # expect 14
	assert(cpu.MEM[200] == 14)

tests_list = [test1, test2]

def tests():
	for t in tests_list:
		t()