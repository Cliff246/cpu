

%pub SQRT
%pub POW
%pub LOG2
%pub LOG10
.text tag std




SQRT:






	jmp.ret null, null, null
POW:



;takes a0 as N
LOG2:
	alu.add t0, zero, zero
LOOP_LOG2:
	alu.srl a0, a0, #1
	alu.add t0, t0, #1
	jmp.ble null, a0, zero, @LOOP_LOG2

	alu.add a0, t0, zero
	jmp.ret null, null, null

LOG10:
	mem.push null, s0, null
	alu.add s0, a0, zero
	;returns on a0
	jmp.call null, null, null, @LOG2
	;load 1 into lhs
	alu.add t1, zero, #1
	;computer 1ull << a0
	alu.sll t0, t1, a0
	;t0 != s0
	jmp.beq null, t0, s0, @ELSE_LOG10
	alu.add a0, a0, #1
ELSE_LOG10:
	mem.pop s0, null, null



;a0 scr1
;a1 src2
;a2 src3
;a2 dst
MATMUL:
	