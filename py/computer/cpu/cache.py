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
	data: list[int]        # instructions stored in this line



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




@dataclass
class ImmCacheLine:
	tag: int
	data: list[int]


class ImmCache(Cache):

	def __init__(self, size):
		super().__init__("ImmCache", size)
		self.lines: dict[int, ImmCacheLine] = {}

	def fill_line(self, tag: int, data: list[int]):
		self.lines[tag] = ImmCacheLine(tag, data)

	def get_line(self, tag: int) -> ImmCacheLine | None:
		return self.lines.get(tag)

	def clear(self):
		self.lines.clear()


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

	def fill_imm_stream(self, tag: int, imms: list[int]):
		self.imm_cache.fill_line(tag, imms)

	def fetch_bundle(self, pc_op: int) -> tuple[list[int] | None, int | None]:
		line = self.inst_cache.get_line(pc_op)
		if line is None:
			return None, None
		imm = None
		if line.imm_mask != 0:
			for slot in range(line.end_pc - line.start_pc):
				if (line.imm_mask & (1 << slot)) != 0:
					pc_imm = line.start_pc + slot
					tag = pc_imm  # For now, tag is pc_imm (could be line start, or other mapping)
					imm_line = self.imm_cache.get_line(tag)
					if imm_line and len(imm_line.data) > 0:
						imm = imm_line.data[0]
					break
		return line.data, imm

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
				0, 0, {}, chunk
			)
			self.inst_cache.fill_line(line)

	def fill_imms(self, tag: int, values: list[int]):
		self.imm_cache.fill_line(tag, values)



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