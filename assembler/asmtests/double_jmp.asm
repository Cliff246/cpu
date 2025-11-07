



.text


START:
	alu.add x0, x0, x0
	jmp.jmp null, null, null, @JMP2


JMP1:
	jmp.jmp null, null, null, @JMP1
JMP2:
	jmp.jmp null, null, null, @JMP2