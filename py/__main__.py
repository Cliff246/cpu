
import tests
from computer.cpu.core import WeirdoCPU

import sys

def load(file):

	binary = []
	with open(file, 'rb') as fp:
		for chunk in iter(lambda: fp.read(8), b""):
			binary.append(int.from_bytes(chunk, byteorder="little", signed=False))

	print(binary)
	wcpu = WeirdoCPU()
	wcpu.load_program(binary)

if __name__  == "__main__":
	if(len(sys.argv) == 1):
		tests.tests()
	else:
		load(sys.argv[1])