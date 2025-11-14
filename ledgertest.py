

import random
from typing import List, Dict, Optional

# ------------------------------------------------------------
# Simplified Fabric + Ledger Simulation
# ------------------------------------------------------------

NUM_REGS = 16          # keep small for visualization
NUM_FABRICS = 4
MAX_EPOCHS = 32        # number of rows in ledger

class Instruction:
    def __init__(self, name, rd, rs_list):
        self.name = name
        self.rd = rd
        self.rs_list = rs_list
        self.src_epochs = []   # filled in by CPU when injected
        self.epoch = None      # row index in the ledger

    def __repr__(self):
        return f"{self.name}(rd=x{self.rd}, rs={self.rs_list}, epoch={self.epoch})"


class Fabric:
    def __init__(self, idx):
        self.idx = idx
        self.q = []

    def inject(self, inst: Instruction):
        self.q.append(inst)

    def step(self):
        if not self.q:
            return None
        return self.q.pop(0)


class Ledger:
    def __init__(self, num_regs, num_epochs):
        self.rows = []
        for _ in range(num_epochs):
            self.rows.append([None for _ in range(num_regs)])

    def set_owner(self, epoch, reg, fabric):
        self.rows[epoch][reg] = fabric

    def snapshot(self, highlight_epoch=None):
        print("\n=== LEDGER SNAPSHOT ===")
        for i, row in enumerate(self.rows):
            marker = "<-- current" if i == highlight_epoch else ""
            line = f"{i:02d} | "
            for cell in row:
                if cell is None:
                    line += ". "
                else:
                    line += f"{cell} "
            print(line, marker)
        print("========================\n")


class CPU:
    def __init__(self):
        self.fabrics = [Fabric(i) for i in range(NUM_FABRICS)]
        self.ledger = Ledger(NUM_REGS, MAX_EPOCHS)
        self.reg_latest_epoch = [0 for _ in range(NUM_REGS)]
        self.current_epoch = 1

    def inject_inst(self, inst: Instruction):
        src_epochs = [self.reg_latest_epoch[r] for r in inst.rs_list]
        inst.src_epochs = src_epochs
        inst.epoch = max(src_epochs) + 1
        self.current_epoch = max(self.current_epoch, inst.epoch)

        fabric_index = random.randint(0, NUM_FABRICS - 1)
        self.fabrics[fabric_index].inject(inst)

        self.ledger.set_owner(inst.epoch, inst.rd, fabric_index)
        self.reg_latest_epoch[inst.rd] = inst.epoch

    def step(self):
        for fab in self.fabrics:
            complete = fab.step()
            if complete:
                pass

        self.ledger.snapshot(highlight_epoch=self.current_epoch)


# ------------------------------------------------------------
# Generate random instructions
# ------------------------------------------------------------

def random_inst():
    rd = random.randint(0, NUM_REGS - 1)
    rs = [random.randint(0, NUM_REGS - 1) for _ in range(random.randint(1, 3))]
    return Instruction("op", rd, rs)

# ------------------------------------------------------------
# MAIN DEMO
# ------------------------------------------------------------

if __name__ == "__main__":
    cpu = CPU()

    for _ in range(20):
        inst = random_inst()
        cpu.inject_inst(inst)
        cpu.step()