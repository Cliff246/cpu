import struct
from dataclasses import dataclass
from collections import deque
from ..memory import Memory
# --- ISA constants (flat bitfields; no micro-decode) ---

from .flags import Flags
from .ALU import ALU
from .registers import Registers
from .cache import *





class WeirdoCPU:




	def __init__(self, mem = None):
		# General-purpose regs: 32 x 64-bit (Python int is unbounded; we mask on writes when needed)
		self.REGS: list[int] = Registers("Register File")
		self.alu_component = ALU("ALU")
		self.dcache = DCache("DCache", 1000)
		self.icache = ICacheSystem("ICache",1000)
		self.components = {self.REGS, self.alu_component, self.dcache, self.icache}


		self.imm_bookmarks: dict[int, int] = {}
		# Program counters (word-indexed)
		self.INSTRUCT_PC: int = 0
		self.IMMEDIATE_PC: int = 0
		self.imm_offset: int = 0
		# Instruction and immediate streams (set via load_program)
		# self.INSTR: list[int] = []		# 32-bit instruction words
		# Map PC -> imm index (how many ImmFlag=1 before this PC)
		# self._IMM_INDEX_OF_PC: list[int] = []
		# Memory that's word addressed
		self.MEM = Memory(1000)

		# Predecoded stream and imm index (no-decode hot path)

		# Frontend "generation" for mispredict recovery
		self.GEN: int = 0

		# Simple frame-bank keyed by the load's PC (demo: 4 lanes of 64b)

		# Load prefetch queue (LPQ) to simulate runahead fetch; each entry completes after ETA steps
		self.LPQ: deque[LpqEntry] = deque()

		# Tiny knob: how many cycles until a prefetch returns
		self.PREFETCH_LATENCY = 2

		# Lookahead window (in instructions) for prefetching future loads
		self.PREFETCH_WINDOW = 8

		# I-cache metadata lines (masks + pre-resolved branch targets)
		# self.IC_LINES: list[ICacheLine] = []

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

	def load_program(self, binary):
		instruction_table_set_base = binary[0]
		instruction_table_set_length = binary[1]
		inst_base = binary[2]
		inst_length = binary[3]
		imm_base = binary[4]
		imm_length = binary[5]

		# Initialize MEM large enough to hold instructions and immediates
		mem_size = max(inst_base + inst_length, imm_base + imm_length, 100000)
		self.MEM = Memory(mem_size)
		# Copy instructions into MEM at inst_base
		count = 0
		for i in binary:
			self.MEM[count] = i
			count += 1


		self.table_base = instruction_table_set_base
		self.table_length = instruction_table_set_length
		self.inst_base = inst_base
		self.inst_length = inst_length
		self.imm_base = imm_base
		self.imm_length = imm_length


		self.INSTRUCT_PC = 0
		self.IMMEDIATE_PC = 0
		self.GEN = 0
		imm_counter = 0
		# Predecode to uops for no-decode hot path

		# Reset frontend structures
		self.dcache.clear()
		self.LPQ.clear()
		# Build I-cache line metadata (imm & branch masks + pre-resolved targets)
		self.icache.clear()
		# Fill ICache with instructions by lines
		n = inst_base + inst_length
		line_start = inst_base
		while line_start < n:
		#	print(line_start)
			line_end = min(n, line_start + Flags.IC_LINE_SIZE)
			if(line_end - line_start > Flags.IC_LINE_SIZE):
				print("fucked")
				exit()
			# Fill instructions for this line from MEM slice
			line = []
			count = 0
			width = 0
			if(n - line_start < Flags.IC_LINE_SIZE):
				width = n - line_start
			else:
				width = Flags.IC_LINE_SIZE
			for i in range(width):
				line.append(self.MEM[line_start + i])
				count += 1
		#	print("line: ", line)
			self.icache.fill_inst(line_start, line)
			line_start = line_end
		# Fill immediates for entire program at once from MEM slice
		imm_line = []
		for i in range(Flags.IC_LINE_SIZE):
			imm_line.append(self.MEM[self.imm_base + i])
		self.icache.fill_imms(0, imm_line)


	#	print("imm cache ",self.icache.imm_cache.lines)
	#	print("inst cache ",self.icache.inst_cache.lines)

	def _imm_at(self) -> int:
		"""
		Return the immediate value for this uop, consuming from imm_cache only once per uop.
		If u.immflag == 0, always return 0.
		Otherwise, if u.imm_val is already set (nonzero or via a marker), return it.
		Else, consume from imm_cache, store in u.imm_val, and return.
		"""
		# If imm_val has already been set (either by nonzero, or by a marker), return it.
		# We use a special marker for valid zero, since imm_val is initialized to 0.
		# We'll use a private attribute on the uop to mark if it was set.

		#print("immedate_pc changed", self.IMMEDIATE_PC)
		tag = self.IMMEDIATE_PC // Flags.IC_LINE_SIZE
		val = self.icache.imm_cache.get_line(tag)
		val = val.data[self.IMMEDIATE_PC % Flags.IC_LINE_SIZE]
		self.imm_offset += 1
		self.IMMEDIATE_PC += 1
		if val is None:
			val = 0
		return val



	def _enqueue_range(self, start_pc: int, end_pc: int):
		"""
		Enqueue prefetches for MEM_LD uops in [start_pc, end_pc).
		This is the primitive used by line- and target-based prefetch to avoid recursion.
		"""
		end_pc = min(end_pc, )
		for pc in range(start_pc, end_pc):
			if u.path != Flags.PATH_MEM or u.subop != Flags.MEM_LD:
				continue
			base = self.REGS[u.rs1]
			imm  = self._imm_at()
			addr = (base + imm) % len(self.MEM)
			base_addr = addr & ~0x3
			fr = self.dcache.get(pc)
			if fr and fr.valid and fr.gen == self.GEN:
				continue
			already = any((e.frame_pc == pc and e.gen == self.GEN) for e in self.LPQ)
			if not already:
				self.LPQ.append(LpqEntry(pc_op=pc, addr=base_addr, size=32, frame_pc=pc, gen=self.GEN, eta=self.PREFETCH_LATENCY))

	def _enqueue_prefetches(self, start_pc: int):
		"""
		Look ahead and enqueue prefetches for MEM_LD ops. Additionally:
		- Prefetch the current -cache line's loads.
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
		#print("Prefetching line", line.start_pc, "for PC", start_pc)
		#print("Also prefetching line", line.start_pc)
	def _drain_lpq(self):
		"""
		Age LPQ, and when entries 'arrive', fill the frame bank (4x64b lanes).
		Wrong-gen entries still warm the cache notionally but do not set dcache valid.
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
				self.dcache[base_addr] = FrameLine(valid=True, gen=self.GEN, data=[d0, d1, d2, d3],source_pc= e.frame_pc, seq_id=0)
			else:
				# Wrong-path arrival: ignore for dcache; treat as a harmless cache warm (no state here).
				pass

	def _ic_line_for_pc(self, pc: int) -> ICacheLine | None:
		if not self.icache.inst_cache.lines:
			return None
		idx = pc // Flags.IC_LINE_SIZE
		if 0 <= idx < len(self.icache.inst_cache.lines):
			return self.icache.inst_cache.lines.get(idx)
		return None

	def _prefetch_by_line(self, start_pc: int):
		"""
		Use I-cache metadata to prefetch loads in an entire line starting at start_pc.
		"""
		line = self._ic_line_for_pc(start_pc)
		if not line:
			return
		self._enqueue_range(line.start_pc, line.end_pc)

	def alu(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int, immflag: bool):
		# Read operands
		a = self.REGS[rs1]
		b = self.REGS[rs2]
		self.REGS[rd] = self.alu_component(subop, a, b, imm, immflag)
		return False

	def memory(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int, immflag: bool):
		if(immflag == False):
			imm = 0
		addr = (self.REGS[rs1] + imm) % len(self.MEM)
		#print(addr)
		if subop == Flags.MEM_LD:
			self.REGS[rd] = self.MEM[self.REGS[rs1] + self.REGS[rs2] + imm]
			#print("LOAD")
			#base_addr = addr & ~0x3
			#lane = addr & 0x3
			#fr = self.dcache.get(base_addr)
			#if fr and fr.valid and fr.gen == self.GEN:
			#	if lane == 0:
			#		self.REGS[rd] = fr.data[0]
			#	elif lane == 1:
			#		self.REGS[rd] = fr.data[1]
			#	elif lane == 2:
			#		self.REGS[rd] = fr.data[2]
			#	elif lane == 3:
			#		self.REGS[rd] = fr.data[3]
			#	else:
			#		self.REGS[rd] = 0
			#	return False"""
			#else:
				# Stall until frame is ready
				#return True

		elif subop == Flags.MEM_ST:
			self.MEM[addr] = self.REGS[rs2]

		else:
			pass
		return False

	def jump_to(self, address):
		#print("jumpto", address)
		#print("immedate_pc_before", self.IMMEDIATE_PC)
		if self.INSTRUCT_PC in self.imm_bookmarks:
			print("BOOK MARKED!", self.INSTRUCT_PC)
			self.IMMEDIATE_PC = self.imm_bookmarks[self.INSTRUCT_PC]
		else:
			print("failed")
			#TODO
			#TO ADD FAILED CACHE SEARCH
			#binary search
			low = 0
			high = (self.table_length // 2)
			closet = 0
			while(low <= high):
				print(closet)
				closet = low + (high - low) // 2
				if(self.MEM[self.table_base + closet * 2] == address):
					closet = closet
				if(self.MEM[self.table_base + closet * 2] < address):
					low = closet + 1
				else:
					high = closet - 1
			#print(closet + self.table_base)
			#print("immedate_ptr %d" % self.MEM[closet + self.table_base + 1])

			#get the offset of the instruction address
			instruction_address = self.MEM[closet + self.table_base]
			#print(instruction_address)
			#immedate address that we are jumping too
			immedate_address = self.MEM[closet + self.table_base + 1]
			#print(immedate_address)
			while(1):
				#very stupid idea....
				#but this is our basic 'well we can mask instructions fast idea' it's not complete but it works
				instruction = self.MEM[instruction_address + self.inst_base]
				#print(immedate_address, instruction_address)
				#our 'mask'
				p, so, rd, rs1, rs2, aux, immf = WeirdoCPU.decode(instruction)
				#print(address, instruction_address)
				#this caused so many problems... drop out before we update
				if(address == instruction_address):
					break
				if immf == 1:
					#print("immedate")
					immedate_address += 1
				#increment the instruction address
				instruction_address += 1
			#set to new immedate address
			self.IMMEDIATE_PC = immedate_address
			#book mark this
			self.imm_bookmarks[self.INSTRUCT_PC] = self.IMMEDIATE_PC

			#self.imm_offset = 0
		self.INSTRUCT_PC = address
		self.print_registers()

		#print("immedate_pc_after", self.IMMEDIATE_PC)


	def branch(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int, immflag: bool) -> bool:
		if subop == Flags.BR_BEQ:
			if self.REGS[rs1] == self.REGS[rs2]:
				self.jump_to(imm)
				# redirect -> new generation
				self.GEN += 1
				# Invalidate dcache from older gens
				#self.dcache = {pc: fr for pc, fr in self.dcache.items() if fr.gen == self.GEN}
				# Cancel LPQ entries from older gens (keep as cache warmers by simply dropping them)
				#self.LPQ = deque(e for e in self.LPQ if e.gen == self.GEN)
				return True
		elif subop == Flags.BR_BNE:
			if self.REGS[rs1] != self.REGS[rs2]:
				self.jump_to(imm)
				self.GEN += 1
				#self.dcache = {pc: fr for pc, fr in self.dcache.items() if fr.gen == self.GEN}
				#self.LPQ = deque(e for e in self.LPQ if e.gen == self.GEN)
				return True
		elif subop == Flags.BR_JMP:
			self.jump_to(imm)
			self.GEN += 1
			#self.dcache = {pc: fr for pc, fr in self.dcache.items() if fr.gen == self.GEN}
			#self.LPQ = deque(e for e in self.LPQ if e.gen == self.GEN)
			return True
		return False

	def system(self, subop: int, rd: int, rs1: int, rs2: int, aux: int, imm: int, imm_flag = 0):
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
			# Drain LPQ completely and invalidate any stale dcache from older gens
			# This models a memory-ordering fence that prevents further speculation.
			while self.LPQ:
				self._drain_lpq()
			# No architectural change to GEN; just ensure dcache are current-gen only.
			self.dcache = {pc: fr for pc, fr in self.dcache.items() if fr.gen == self.GEN}
			# Return 0 in rd to indicate fence completed
			self.REGS[rd] = 0
		else:
			# Unknown SYS op: no state change
			return False

	def step(self):
		if self.INSTRUCT_PC < 0 or self.INSTRUCT_PC >= self.inst_length:
			return  # Halt when PC runs past program
		#print("instruction(%d) immedate(%d)" %( self.IMMEDIATE_PC, self.INSTRUCT_PC))
		# Lightweight frontend: look ahead and enqueue prefetches; drain LPQ to fill dcache


		#self._enqueue_prefetches(self.INSTRUCT_PC)
		#self._drain_lpq()


		#decode 'ish'
		ins = self.MEM[self.INSTRUCT_PC + self.inst_base]
		path, subop, rd, rs1, rs2, aux, immf = self.decode(ins)

		#debugging
		self.print_registers()
		WeirdoCPU.print_op_information(ins)

		#print("path:%d subop:%d rd:%d rs1:%d rs2:%d aux:%d immf:%d" % (path,subop, rd, rs1, rs2, aux, immf))
		#print("immaddress", self.IMMEDIATE_PC)

		if immf == 1:
			#fill cache line if not functional
			if self.IMMEDIATE_PC // Flags.IC_LINE_SIZE not in self.icache.imm_cache.lines.keys():
				tag = self.IMMEDIATE_PC // Flags.IC_LINE_SIZE
				line_base = self.imm_base + (self.IMMEDIATE_PC // Flags.IC_LINE_SIZE) * Flags.IC_LINE_SIZE
				print(line_base)
				pull = [self.MEM[line_base + i] for i in range(Flags.IC_LINE_SIZE)]
				self.icache.fill_imm_stream(tag, pull)
			imm = self._imm_at()
		else:
			imm = 0

		#print(imm)
		taken = False
		if path == Flags.PATH_ALU:
			taken = self.alu(subop, rd, rs1, rs2, aux, imm, immf)
		elif path == Flags.PATH_MEM:
			taken = self.memory(subop,rd, rs1, rs2, aux, imm, immf)

		elif path == Flags.PATH_BR:
			taken = self.branch(subop, rd, rs1, rs2, aux, imm, immf)
		elif path == Flags.PATH_SYS:
			taken = self.system(subop, rd, rs1, rs2, aux, imm, immf)
		else:
			pass


		if(taken == False):
			self.INSTRUCT_PC += 1

	@staticmethod
	def print_op_information(ins):
		path, subop, rd, rs1, rs2, aux, immf = WeirdoCPU.decode(ins)
		path_str = Flags.convert_to_string_path(path)
		subop_str = Flags.convert_subop(path, subop)
		print("%s %s %d %d %d %d %d" % (path_str, subop_str, rd, rs1, rs2, aux, immf))

	def print_registers(self):
		print("r1:%d r2:%d r3:%d r4:%d r5:%d r6:%d" % ( self.REGS[1], self.REGS[2], self.REGS[3], self.REGS[4], self.REGS[5], self.REGS[6]))

