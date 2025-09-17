from dataclasses import dataclass
from collections import deque

from .component import Component

from .flags import Flags

@dataclass
class ICacheLine:
	start_pc: int		  # inclusive PC index of first op in this line
	end_pc: int			# exclusive PC index (start_pc + up to IC_LINE_SIZE)
	imm_mask: int		  # bit i set => slot i consumes an imm-tape entry
	br_mask: int		   # bit i set => slot i is a branch/jump
	br_targets: dict[int, int]  # slot_index -> target_pc (PC-relative already resolved)



@dataclass
class FrameLine:
	valid: bool
	gen: int
	data: list[int]
	source_pc: int
	seq_id: int


@dataclass
class LpqEntry:
	pc_op: int
	addr: int
	size: int   # bytes (we'll treat as 32B for a 4x64b frame)
	frame_pc: int
	gen: int
	eta: int	# cycles until "arrives"



class Cache(Component):

	def __init__(self, component_type, size):
		super().__init__(component_type)
		self.size = size


	def get_size(self):
		return self.size

	def len(self):
		return self.size

class InstCache(Cache):
	def __init__(self, size):
		super().__init__("InstCache", size)
		self.lines: dict[int, ICacheLine] = {}

	def fill_line(self, line: ICacheLine):
		self.lines[line.start_pc] = line

	def get_line(self, pc_op: int) -> ICacheLine | None:
		return self.lines.get(pc_op)

	def clear(self):
		self.lines.clear()




class ImmCache(Cache):

	def __init__(self, size):
		super().__init__("ImmCache", size)
		self.imms: list[int] = []
		self.cursor: int = 0
		self.bookmarks: dict[int, int] = {}

	def fill_tape(self ,imms: list[int]):
		self.imms = imms
		self.cursor = 0
		self.bookmarks.clear()

	def next(self, pc: int) -> int | None:
		if self.cursor >= len(self.imms):
			return None
		imm = self.imms[self.cursor]
		if pc not in self.bookmarks:
			self.bookmarks[pc] = self.cursor
		self.cursor += 1
		return imm

	def jump_to(self, pc: int):
		if pc in self.bookmarks:
			self.cursor = self.bookmarks[pc]

	def clear(self):
		self.imms.clear()
		self.cursor = 0
		self.bookmarks.clear()

	def fetch_imm(self, pc):
		return self.next(pc)

class ICacheSystem(Component):

	def __init__(self, size_inst: int, size_imm: int):
		super().__init__("ICacheSystem")
		self.inst_cache = InstCache(size_inst)
		self.imm_cache = ImmCache(size_imm)

	def fill_inst_line(self, line: ICacheLine):
		self.inst_cache.fill_line(line)
		# If line has immediates (imm_mask), we assume parallel fill for imm cache
		if line.imm_mask != 0:
			# For simplicity, we assume immediates are provided externally to fill_stream
			# This method does not fill imm cache directly, user should call fill_imm_stream separately
			pass

	def fill_imm_stream(self, pc_imm: int, imms: list[int]):
		self.imm_cache.fill_tape(pc_imm, imms)

	def fetch_bundle(self, pc_op: int) -> tuple[ICacheLine | None, int | None]:
		line = self.inst_cache.get_line(pc_op)
		if line is None:
			return None, None
		# Determine if an immediate is needed for first slot with imm_mask bit set
		imm = None
		if line.imm_mask != 0:
			# Find first slot with imm_mask bit set
			for slot in range(line.end_pc - line.start_pc):
				if (line.imm_mask & (1 << slot)) != 0:
					pc_imm = line.start_pc + slot
					imm = self.imm_cache.fetch_imm(pc_imm)
					break
		return line, imm

	def clear(self):
		self.inst_cache.clear()
		self.imm_cache.clear()


	def fill_inst(self, start_pc, instrs):
		n = len(instrs)
		for i in range(0, n, Flags.IC_LINE_SIZE):
			chunk = instrs[i:i+Flags.IC_LINE_SIZE]
			line = ICacheLine(
				start_pc + i,
				start_pc + i + len(chunk),
				0, 0, {}
			)
			self.inst_cache.fill_line(line)

	def fill_imms(self, values):
		self.imm_cache.fill_tape(values)



class DCache(Cache):
	def __init__(self, component_type, size):
		super().__init__("DCache", size)

		self.frames: dict[int, FrameLine] = {}

	def __getitem__(self, key):
		return self.frames[key]

	def __setitem__(self, key, value):
		self.frames[key] = value

	def get(self, key, default=None):
		return self.frames.get(key, default)

	def clear(self):
		self.frames.clear()

	def items(self):
		return self.frames.items()

	def keys(self):
		return self.frames.keys()