

%imp "asmtests/testsuit/test_combine/combine2.asm"
%def JMP1
%order combine
.text tag combine start


JMP2:
	jmp.jmp null, null, null, @JMP1