

%pub strlen
%pub strcpy
%pub strcmp
%pub memcpy
%pub memset
%pub strncpy
%pub strindex


.text tag std



;a0 = string
;returns length
strlen:
;TODO

	;ptr
	alu.add t0, a0, zero
	;total counter
	alu.add t2, zero, zero


strlen_loop:
	mem.ld t3, t0, zero



	alu.add t1, zero, #8
strlen_char_loop:

	alu.and t4, t3, null, 0xff

	jmp.beq null, t4, zero, @strlen_end
	alu.srl t3, t3, #8


	alu.sub t1, t1, #1
	alu.add t2, t2, #1

	jmp.bne null, t1, zero, @strlen_char_loop

	alu.add t0, t0, #1
	jmp.jmp null, null, null, @strlen_loop


strlen_end:
	alu.add a0, t2, zero
	jmp.ret null, null, null

;a0 = src1
;a1 = src2
strcpy:
;TODO
	alu.add t0, a0, zero
	alu.add t1, a1, zero


strcpy_loop:



strcpy_end:


	jmp.ret null, null, null

;a0 = dst1
;a1 = src2
strcmp:
;TODO
	alu.add t0, a0, zero
	alu.add t1, a1, zero


	;counter
	alu.add t2, zero, zero
strcpy_loop:

	mem.ld t3, t1, zero

strcpy_check:




strcmp_end:
	jmp.ret null, null, null

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


