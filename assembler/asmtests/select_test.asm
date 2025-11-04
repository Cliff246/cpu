%def MAIN
%imp "asmtests/strlen.asm"
%def SQRT_FUNCTION
.text tag test



START:
	alu.add x9, zero, zero, 100
	alu.sub x10, x9, #60
	alu.add x10, x5, x5, @SQRT_FUNCTION