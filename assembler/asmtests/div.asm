
.text

MAIN:

	alu.add t0, zero, zero, 10
	alu.add t2, zero, zero, -5
	alu.div! t3, t0, t2
	alu.add x0, x0, x0
