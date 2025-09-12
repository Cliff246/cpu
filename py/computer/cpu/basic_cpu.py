import struct
from dataclasses import dataclass
from collections import deque
from ..memory import Memory
# --- ISA constants (flat bitfields; no micro-decode) ---

from .flags import Flags
from .ALU import ALU
from .registers import Registers
@dataclass
class ICacheLine:
	start_pc: int		  # inclusive PC index of first op in this line
	end_pc: int			# exclusive PC index (start_pc + up to IC_LINE_SIZE)
	imm_mask: int		  # bit i set => slot i consumes an imm-tape entry
	br_mask: int		   # bit i set => slot i is a branch/jump
	br_targets: dict[int, int]  # slot_index -> target_pc (PC-relative already resolved)

@dataclass
class Uop:
	path: int
	subop: int
	rd: int
	rs1: int
	rs2: int
	aux: int
	immflag: int   # 0/1
	imm_idx: int   # index into IMM_TAPE for this PC (or -1 if immflag=0)

@dataclass
class FrameLine:
	valid: bool
	gen: int
	d0: int
	d1: int
	d2: int
	d3: int

@dataclass
class LpqEntry:
	pc_op: int
	addr: int
	size: int   # bytes (we'll treat as 32B for a 4x64b frame)
	frame_pc: int
	gen: int
	eta: int	# cycles until "arrives"




class WeirdoCPU:




	def __init__(self, mem = None):
		# General-purpose regs: 32 x 64-bit (Python int is unbounded; we mask on writes when needed)
		self.REGS: list[int] = Registers("Register File")
		self.alu_component = ALU("ALU")

		self.components = {self.REGS, self.alu_component}

		# Program counters (word-indexed)
		self.INSTRUCT_PC = 0
		# Instruction and immediate streams (set via load_program)
		self.INSTR: list[int] = []		# 32-bit instruction words
		self.IMM_TAPE: list[int] = []	 # 64-bit immediates
		# Map PC -> imm index (how many ImmFlag=1 before this PC)
		self._IMM_INDEX_OF_PC: list[int] = []
		# Memory that's word addressed
		self.MEM = Memory(1000)

		# Predecoded stream and imm index (no-decode hot path)
		self.UOPS: list[Uop] = []

		# Frontend "generation" for mispredict recovery
		self.GEN: int = 0

		# Simple frame-bank keyed by the load's PC (demo: 4 lanes of 64b)
		self.FRAMES: dict[int, FrameLine] = {}

		# Load prefetch queue (LPQ) to simulate runahead fetch; each entry completes after ETA steps
		self.LPQ: deque[LpqEntry] = deque()

		# Tiny knob: how many cycles until a prefetch returns
		self.PREFETCH_LATENCY = 2

		# Lookahead window (in instructions) for prefetching future loads
		self.PREFETCH_WINDOW = 8

		# I-cache metadata lines (masks + pre-resolved branch targets)
		self.IC_LINES: list[ICacheLine] = []

		# Frontend/preload controls
		self.PREFETCH_ENABLED = True
		self.FENCE_PENDING = False


	@staticmethod
	def decode(instr):
		#4 bit so 16 path items

		path = (instr >> 28) & 0xF
		subop = (instr >> 20) & 0xFF
		rd = (instr >> 15) & 0x1F
		rs1 = (instr >> 10) & 0x1F
		rs2 = (instr >> 5) & 0x1F
		aux = (instr >> 1) & 0xF
		immflag = instr & 0x1

		return path, subop, rd, rs1, rs2, aux, immflag

	@staticmethod
	def print_decode(decoded):
		path, subop, rd, rs1, rs2, aux, immflag = decoded
		print("path: %d, subop: %d, rd: %d, rs1: %d, rs2: %d, aux: %d, immflag %d" % (path, subop, rd, rs1, rs2, aux, immflag))

	@staticmethod
	def _build_imm_index(instrs: list[int]) -> list[int]:
		idx = [0] * (len(instrs) + 1)
		c = 0
		for pc, ins in enumerate(instrs):
			idx[pc] = c
			if (ins & 0x1) == 1:  # ImmFlag in bit 0
				c += 1
		idx[len(instrs)] = c
		return idx

	def load_program(self, instrs: list[int], imms: list[int]):
		self.INSTR = instrs[:]
		self.IMM_TAPE = imms[:]
		self._IMM_INDEX_OF_PC = self._build_imm_index(self.INSTR)
		self.INSTRUCT_PC = 0
		self.GEN = 0
		self.UOPS = []
		# Predecode to uops for no-decode hot path
		for pc, ins in enumerate(self.INSTR):
			path, subop, rd, rs1, rs2, aux, immf = self.decode(ins)
			imm_idx = self._IMM_INDEX_OF_PC[pc] if immf else -1
			self.UOPS.append(Uop(path, subop, rd, rs1, rs2, aux, immf, imm_idx))
		# Reset frontend structures
		self.FRAMES.clear()
		self.LPQ.clear()
		# Build I-cache line metadata (imm & branch masks + pre-resolved targets)
		self.IC_LINES = []
		n = len(self.UOPS)
		line_start = 0
		while line_start < n:
			line_end = min(n, line_start + Flags.IC_LINE_SIZE)
			imm_mask = 0
			br_mask = 0
			br_targets: dict[int, int] = {}
			for i, pc in enumerate(range(line_start, line_end)):
				u = self.UOPS[pc]
				if u.immflag:
					imm_mask |= (1 << i)
				if u.path == Flags.PATH_BR and (u.subop in (Flags.BR_BEQ, Flags.BR_BNE, Flags.BR_JMP)):
					br_mask |= (1 << i)
					# Resolve target PC using imm tape (PC-relative for BEQ/BNE; absolute for JMP via rs1)
					if u.subop == Flags.BR_JMP:
						# We cannot know rs1 at build time; mark with -1 to indicate "unknown"
						br_targets[i] = -1
					else:
						rel = self._imm_at(u)
						br_targets[i] = pc + rel
			self.IC_LINES.append(ICacheLine(start_pc=line_start, end_pc=line_end,
											imm_mask=imm_mask, br_mask=br_mask, br_targets=br_targets))
			line_start = line_end

	def _imm_at(self, u: Uop) -> int:
		if u.immflag == 0:
			return 0
		idx = u.imm_idx
		if idx < 0 or idx >= len(self.IMM_TAPE):
			raise IndexError("IMM tape underflow/overflow")
		return self.IMM_TAPE[idx]

	def _enqueue_range(self, start_pc: int, end_pc: int):
		"""
		Enqueue prefetches for MEM_LD uops in [start_pc, end_pc).
		This is the primitive used by line- and target-based prefetch to avoid recursion.
		"""
		end_pc = min(end_pc, len(self.UOPS))
		for pc in range(start_pc, end_pc):
			u = self.UOPS[pc]
			if u.path != Flags.PATH_MEM or u.subop != Flags.MEM_LD:
				continue
			base = self.REGS[u.rs1]
			imm  = self._imm_at(u)
			addr = (base + imm) % len(self.MEM)
			base_addr = addr & ~0x3
			fr = self.FRAMES.get(pc)
			if fr and fr.valid and fr.gen == self.GEN:
				continue
			already = any((e.frame_pc == pc and e.gen == self.GEN) for e in self.LPQ)
			if not already:
				self.LPQ.append(LpqEntry(pc_op=pc, addr=base_addr, size=32, frame_pc=pc, gen=self.GEN, eta=self.PREFETCH_LATENCY))

	def _enqueue_prefetches(self, start_pc: int):
		"""
		Look ahead and enqueue prefetches for MEM_LD uops. Additionally:
		- Prefetch the current I-cache line's loads.
		- Prefetch the next sequential line's loads.
		- If the current line contains a branch with a statically-known target (BEQ/BNE rel),
		  also prefetch that target line's loads.
		"""
		if not self.PREFETCH_ENABLED:
			return

		line = self._ic_line_for_pc(start_pc)
		if line:
			# 1) Prefetch loads within the current line window
			self._enqueue_range(line.start_pc, line.end_pc)
			# 2) Prefetch next sequential line
			next_line = self._ic_line_for_pc(line.end_pc)
			if next_line:
				self._enqueue_range(next_line.start_pc, next_line.end_pc)
			# 3) Speculative prefetch for statically-known branch targets in this line
			if line.br_mask:
				for slot, tgt in line.br_targets.items():
					if tgt is None or tgt < 0:
						continue
					tgt_line = self._ic_line_for_pc(tgt)
					if tgt_line:
						self._enqueue_range(tgt_line.start_pc, tgt_line.end_pc)
		else:
			# Fallback: simple window-based lookahead
			end_pc = min(len(self.UOPS), start_pc + self.PREFETCH_WINDOW)
			self._enqueue_range(start_pc, end_pc)

	def _drain_lpq(self):
		"""
		Age LPQ, and when entries 'arrive', fill the frame bank (4x64b lanes).
		Wrong-gen entries still warm the cache notionally but do not set frames valid.
		Draining can be invoked by SYS_FENCE to enforce ordering.
		"""
		if not self.LPQ:
			return
		# decrement ETA and collect arrivals
		for i in range(len(self.LPQ)):
			e = self.LPQ[i]
			if e.eta > 0:
				self.LPQ[i] = LpqEntry(e.pc_op, e.addr, e.size, e.frame_pc, e.gen, e.eta - 1)
		arrivals = []
		while self.LPQ and self.LPQ[0].eta == 0:
			arrivals.append(self.LPQ.popleft())
		for e in arrivals:
			# "Load" 4x64b from memory starting at addr (wrap memory for demo)
			# This simulates a 32B read landing into a frame with 4 lanes.
			if e.gen == self.GEN:
				d0 = self.MEM[e.addr % len(self.MEM)]
				d1 = self.MEM[(e.addr + 1) % len(self.MEM)]
				d2 = self.MEM[(e.addr + 2) % len(self.MEM)]
				d3 = self.MEM[(e.addr + 3) % len(self.MEM)]
				base_addr = e.addr & ~0x3
				self.FRAMES[base_addr] = FrameLine(valid=True, gen=self.GEN, d0=d0, d1=d1, d2=d2, d3=d3)
			else:
				# Wrong-path arrival: ignore for frames; treat as a harmless cache warm (no state here).
				pass

	def _ic_line_for_pc(self, pc: int) -> ICacheLine | None:
		if not self.IC_LINES:
			return None
		idx = pc // Flags.IC_LINE_SIZE
		if 0 <= idx < len(self.IC_LINES):
			return self.IC_LINES[idx]
		return None

	def _prefetch_by_line(self, start_pc: int):
		"""
		Use I-cache metadata to prefetch loads in an entire line starting at start_pc.
		"""
		line = self._ic_line_for_pc(start_pc)
		if not line:
			return
		self._enqueue_range(line.start_pc, line.end_pc)

	def alu(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int):
		# Read operands
		a = self.REGS[rs1]
		b = self.REGS[rs2]
		self.REGS[rd] = self.alu_component(subop, a, b, imm)

	def memory(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int):
		addr = (self.REGS[rs1] + imm) % len(self.MEM)
		cur_pc = self.INSTRUCT_PC
		if subop == Flags.MEM_LD:
			base_addr = addr & ~0x3
			lane = addr & 0x3
			fr = self.FRAMES.get(base_addr)
			if fr and fr.valid and fr.gen == self.GEN:
				if lane == 0:
					self.REGS[rd] = fr.d0
				elif lane == 1:
					self.REGS[rd] = fr.d1
				elif lane == 2:
					self.REGS[rd] = fr.d2
				elif lane == 3:
					self.REGS[rd] = fr.d3
				else:
					self.REGS[rd] = 0
				return False
			else:
				# Stall until frame is ready
				return True
		elif subop == Flags.MEM_ST:
			self.MEM[addr] = self.REGS[rs2]
		else:
			pass
		return False

	def branch(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int) -> bool:
		if subop == Flags.BR_BEQ:
			if self.REGS[rs1] == self.REGS[rs2]:
				self.INSTRUCT_PC += imm
				# redirect -> new generation
				self.GEN += 1
				# Invalidate frames from older gens
				self.FRAMES = {pc: fr for pc, fr in self.FRAMES.items() if fr.gen == self.GEN}
				# Cancel LPQ entries from older gens (keep as cache warmers by simply dropping them)
				self.LPQ = deque(e for e in self.LPQ if e.gen == self.GEN)
				return True
		elif subop == Flags.BR_BNE:
			if self.REGS[rs1] != self.REGS[rs2]:
				self.INSTRUCT_PC += imm
				self.GEN += 1
				self.FRAMES = {pc: fr for pc, fr in self.FRAMES.items() if fr.gen == self.GEN}
				self.LPQ = deque(e for e in self.LPQ if e.gen == self.GEN)
				return True
		elif subop == Flags.BR_JMP:
			self.INSTRUCT_PC = self.REGS[rs1]
			self.GEN += 1
			self.FRAMES = {pc: fr for pc, fr in self.FRAMES.items() if fr.gen == self.GEN}
			self.LPQ = deque(e for e in self.LPQ if e.gen == self.GEN)
			return True
		return False

	def system(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int):
		# System/control path: toggle/speculate barrier and fences
		if subop == Flags.SYS_PREFETCH_OFF:
			self.PREFETCH_ENABLED = False
			# write a simple status code to rd for visibility (1 = off)
			self.REGS[rd] = 1
		elif subop == Flags.SYS_PREFETCH_ON:
			self.PREFETCH_ENABLED = True
			# status code (0 = on)
			self.REGS[rd] = 0
		elif subop == Flags.SYS_FENCE:
			# Drain LPQ completely and invalidate any stale frames from older gens
			# This models a memory-ordering fence that prevents further speculation.
			while self.LPQ:
				self._drain_lpq()
			# No architectural change to GEN; just ensure frames are current-gen only.
			self.FRAMES = {pc: fr for pc, fr in self.FRAMES.items() if fr.gen == self.GEN}
			# Return 0 in rd to indicate fence completed
			self.REGS[rd] = 0
		else:
			# Unknown SYS op: no state change
			pass

	def step(self):
		if self.INSTRUCT_PC < 0 or self.INSTRUCT_PC >= len(self.UOPS):
			return  # Halt when PC runs past program

		# Lightweight frontend: look ahead and enqueue prefetches; drain LPQ to fill frames
		self._enqueue_prefetches(self.INSTRUCT_PC)
		self._drain_lpq()

		u = self.UOPS[self.INSTRUCT_PC]
		imm = self._imm_at(u) if u.immflag else 0

		taken = False
		if u.path == Flags.PATH_ALU:
			self.alu(u.subop, u.rd, u.rs1, u.rs2, u.aux, imm)
		elif u.path == Flags.PATH_MEM:
			taken = self.memory(u.subop, u.rd, u.rs1, u.rs2, u.aux, imm)

		elif u.path == Flags.PATH_BR:
			taken = self.branch(u.subop, u.rd, u.rs1, u.rs2, u.aux, imm)
		elif u.path == Flags.PATH_SYS:
			taken = self.system(u.subop, u.rd, u.rs1, u.rs2, u.aux, imm)
		else:
			pass

		# Insert trace after execution, before incrementing PC
		self.trace_state()

		if not taken:
			self.INSTRUCT_PC += 1


	def trace_state(self):
		if self.INSTRUCT_PC < 0 or self.INSTRUCT_PC >= len(self.UOPS):
			print("PC out of range:", self.INSTRUCT_PC)
			return
		u = self.UOPS[self.INSTRUCT_PC]
		imm = self._imm_at(u) if u.immflag else 0
		print(f"PC={self.INSTRUCT_PC} GEN={self.GEN} | path={u.path} subop={u.subop} rd={u.rd} rs1={u.rs1} rs2={u.rs2} aux={u.aux} immf={u.immflag} imm={imm}")
		# Show a compact register file view (first 8 registers)
		regs_preview = " ".join(f"r{i}={self.REGS[i]}" for i in range(8))
		print("REGS:", regs_preview)
		# Show LPQ depth and outstanding frame PCs
		print(f"LPQ depth={len(self.LPQ)} FRAMES={[pc for pc in self.FRAMES.keys()]}")
