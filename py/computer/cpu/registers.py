
from .component import Component

class Registers(Component):

	def __init__(self, component_type):
		super().__init__(component_type)
		self.regs = [0] * 32

	def __getitem__(self, instance):
		if not isinstance(instance, int):
			raise TypeError("key for register must be int")
		if(instance < 0 or instance >= 32):
			raise KeyError("register was attempted that's not possible")
		if(instance == 0):
			return 0
		else:
			return self.regs[instance]

	def __setitem__(self, instance, value):
		if not isinstance(instance, int) and isinstance(value, int):
			raise TypeError("key for register must be int")
		if(instance < 0 or instance >= 32):
			raise KeyError("register was attempted that's not possible")
		if(instance == 0):
			return
		else:
			self.regs[instance] = value