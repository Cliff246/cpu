



class Memory:


	def __init__(self, size: int ):
		self.sizemem = size
		self.content = [0] * self.sizemem


	def __getitem__(self, instance):
		return self.load(instance)

	def __setitem__(self, name, value):
		self.store(name, value)



	@staticmethod
	def check_in_range(value):
		if isinstance(value, int):
			if(value > 2147483647 or value < -2147483648):
				return False
			return True
		else:
			raise TypeError("Value is not of type int")


	def load(self, address) -> int:
		if not isinstance(address, int):
			print("address was of wrong python type")
			exit(1)
		if(address >= self.sizemem or address < 0):
			raise KeyError("address not in range")
		value = self.content[address]
		return value

	def store(self, address, value):
		if(address >= self.sizemem or address < 0):
			raise KeyError("address not in range")
		else:
			try:
				if(self.check_in_range(value)):
					self.content[address] = value
				else:
					value = value & 0xffffffff
					self.content[address] = value
			except TypeError:
				print("value was of wrong python type")
				exit(1)


	def __len__(self):
		return self.sizemem