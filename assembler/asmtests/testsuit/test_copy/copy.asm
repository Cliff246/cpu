
%pub COPY
.text tag std

;takes a0 as dest
;takes a1 as src
;takes a2 as size
COPY:
	;set the ptr to store at
	alu.add t0, a0, zero
	;set the ptr to load at
	alu.add t1, a1, zero
	alu.add t2, a2, zero

JMP_LEN:
	;load the src at t1 offset
	mem.ld t4, t1, zero
	;store t4 data at t0
	mem.st zero, t4, t0
	;add to src and dest
	alu.add t0, t0, #1
	alu.add t1, t1, #1
	;subtract counter
	alu.sub t2, t2, #1

	jmp.bne null, t2, zero, @JMP_LEN

	jmp.ret zero, zero, zero


