
%imp "asmtests/testsuit/test_combine/combine1.asm"
%pub JMP1
.text tag combine

START:
	alu.add t0, t1, #5
JMP1:
	alu.add t0, t0, t0

	
