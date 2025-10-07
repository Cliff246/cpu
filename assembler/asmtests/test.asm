.text
run:
	jmp.jmp x0, x0, x0, open
	alu.div zero, x0, x0
	alu.add x0, x0, x0, run
	alu.add x0, x0, x0, again
again:
	alu.add x10, x5, x0, again
open:
	alu.add x0, x0, x0
	jmp.jmp x0, x0, x0, again