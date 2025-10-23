
.text



MAIN:
	;co
	alu.add t0, zero, zero, 1

	;table +t0
	jmp.jmp null, null, t0, @TABLE

TABLE:

	;0
	jmp.jmp null, null, null, @JMP1
	;1
	jmp.jmp null, null, null, @JMP2
	;2
	jmp.jmp null, null, null, @JMP3


JMP1:
	jmp.jmp null, null, null, @JMP1
JMP2:
	jmp.jmp null, null, null, @JMP2

JMP3:
	jmp.jmp null, null, null, @JMP3



