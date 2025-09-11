from cpu import *

def encode(path, subop, rd, rs1, rs2, aux=0, immflag=0):
    """Pack an instruction into your 32-bit layout"""
    return ((path & 0xF) << 28) | ((subop & 0xFF) << 20) | ((rd & 0x1F) << 15) \
           | ((rs1 & 0x1F) << 10) | ((rs2 & 0x1F) << 5) | ((aux & 0xF) << 1) \
           | (immflag & 0x1)

# --- program ---
instrs = []
imms   = []

# r1 = MEM[100]
instrs.append(encode(PATH_MEM, MEM_LD, rd=1, rs1=0, rs2=0, immflag=1))
imms.append(100)

# r2 = MEM[101]
instrs.append(encode(PATH_MEM, MEM_LD, rd=2, rs1=0, rs2=0, immflag=1))
imms.append(101)

# r3 = r1 + r2
instrs.append(encode(PATH_ALU, ALU_ADD, rd=3, rs1=1, rs2=2))

# MEM[102] = r3
instrs.append(encode(PATH_MEM, MEM_ST, rd=0, rs1=0, rs2=3, immflag=1))
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