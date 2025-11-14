
import sys

args = sys.argv




if(len(args) != 3):
	print("must have arguments be 2")



with open(args[1], "rb") as f1, open(args[2], "rb") as f2:
	b1, b2 = f1.read(), f2.read()

	if(b1 == b2):
		print("files are exactly the same")
	else:

		print("files differ")
		for i, (a, b) in enumerate(zip(b1, b2)):
			if a != b:
				print(f"difference at byte {i:08x} : {a:02x} vs {b:02x}")
				
