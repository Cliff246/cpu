

%pub

.text tag std



;a0 = string
;returns length
strlen:
;TODO
	

;a0 = src1
;a1 = src2

strcpy:
;TODO


;a0 = src1
;a1 = src2
strcmp:
;TODO



;a0 = src1
;a1 = src2
;a2 = length
strncpy:
;TODO

;a0 = source
;a1 = index
strindex:
;TODO


;a0 = dest
;a1 = src
;a2 = size
memcpy:
	alu.add t0, a0, zero
	alu.add t1, a1, zero
	alu.add t2, a2, zero
memcpy_loop:
	;load the src at t1 offset
	mem.ld t4, t1, zero
	;store t4 data at t0
	mem.st! t4, t0, zero
	;add to src and dest
	alu.add t0, t0, #1
	alu.add t1, t1, #1
	;subtract counter
	alu.sub t2, t2, #1
	jmp.bne null, t2, zero, @memcpy_loop
	jmp.ret zero, zero, zero





;a0 = source
;a1 = value
;a2 = length
memset:


