.config
	pub MAINS
	inc "sqrt.asm"
	def "SQRT_FUNCTION"
.text

MAIN: test
	%add x5, x0, #5
	alu.add x9, zero, zero, 100
	alu.sub x10, x9, #60