from computer import *
from .assembler_for_now import _assemble, instruction


def test1():

	# --- program ---
	instrs = []

	# r1 = MEM[100]
	instrs.append(instruction(path=Flags.PATH_MEM, subop=Flags.MEM_LD, rd=1, rs1=0, rs2=0, aux=0,immflag=1, imm=100))
	# r2 = MEM[101]
	instrs.append(instruction(path=Flags.PATH_MEM, subop=Flags.MEM_LD, rd=2, rs1=0, rs2=0, aux=0, immflag=1, imm = 101))
	# r3 = r1 + r2
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=3, rs1=1, rs2=2, aux= 0))
	# MEM[102] = r3
	instrs.append(instruction(path=Flags.PATH_MEM, subop=Flags.MEM_ST, rd=0, rs1=0, rs2=3, aux = 0,immflag=1, imm= 102))

	# immediates corresponding to instructions with immflag=1

	# assemble instructions to binary
	binary = _assemble(instrs)

	# --- run it ---
	cpu = WeirdoCPU()
	cpu.load_program(binary)
	print(cpu.MEM.content)

	# preload memory
	cpu.MEM[100] = 7
	cpu.MEM[101] = 5
	I = 0
	while I < 100:
		cpu.step()
		I += 1

	print("MEM[102] =", cpu.MEM[102])  # expect 12
	assert(cpu.MEM[102] == 12)

def test2():

	# registers
	r0=0; r1=1; r2=2; r3=3; r4=4; r5=5; r6=6

	instrs = []
	imms = []

	# Assembler handles encoding and binary layout

	# r2 = base (100)
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=r2, rs1=r0, rs2=0, aux=0,immflag=1, imm = 100))
	# r3 = count (5)
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=r3, rs1=r0, rs2=0,aux=0, immflag=1, imm = 5))
	# r4 = 0 (accum)
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=r4, rs1=r0, rs2=0,aux=0, immflag=1, imm = 0))

	loop = len(instrs)

	# LD r5, [r2 + 0]
	instrs.append(instruction(path=Flags.PATH_MEM, subop=Flags.MEM_LD, rd=r5, rs1=r2, rs2=0, aux=0,immflag=0))
	# r4 += r5
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=r4, rs1=r4, rs2=r5, aux= 0))
	# r2 += 1 (next element)
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=r2, rs1=r2, rs2=0, aux=0, immflag=1, imm = 1))
	# r3 += -1
	instrs.append(instruction(path=Flags.PATH_ALU, subop=Flags.ALU_ADD, rd=r3, rs1=r3, rs2=0,aux=0, immflag=1, imm = -1))
	# if r3 != 0, branch back to loop
	branch_pc = len(instrs)  # the index this branch will sit at
	back = loop

	instrs.append(instruction(path=Flags.PATH_BR, subop=Flags.BR_BNE, rd=0, rs1=r3, rs2=r0, aux=0,immflag=1, imm=back))

	# store sum at MEM[200]
	instrs.append(instruction(path=Flags.PATH_MEM, subop=Flags.MEM_ST, rd=0, rs1=r6, rs2=r4, aux=0, immflag=1, imm=0))

	# assemble instructions to binary
	binary = _assemble(instrs)
	#print(binary[7:len(binary)])
	#print(len(binary))
	#breakpoint()
	cpu = WeirdoCPU()
	cpu.load_program(binary)

	cpu.REGS[r6] = 200
	base=100
	for i,v in enumerate([3,1,4,1,5]):
		cpu.MEM[base+i] = v

	# run
	I = 0
	while cpu.INSTRUCT_PC < len(instrs) and I < 100:
		cpu.step()
		I+=1

	print("sum -> MEM[200] =", cpu.MEM[200])  # expect 14
	assert(cpu.MEM[200] == 14)

tests_list = [test1,test2]#, test2]

def tests():
	for t in tests_list:
		t()