.config
	pub MAIN
	include "sqrt.asm"
	def "SQRT_FUNCTION"
.text

MAIN: test
	alu.add x5, x0, #5
	alu.add x9, zero, zero, 100
	alu.sub x10, x9, #60