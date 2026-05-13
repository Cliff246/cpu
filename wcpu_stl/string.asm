

%pub strlen
%pub strcpy
%pub strcmp
%pub memcpy
%pub memset
%pub strncpy
%pub strindex
%pub setindex
%pub reverse

.text tag std


;---------------------------------

;a0 = string
;returns length
strlen:
;TODO

	;ptr
	alu.add t0, a0, zero
	;total counter
	alu.add t2, nil, zero


strlen_loop:
	mem.ld t3, t0, zero



	alu.add t1, zero, #8
strlen_char_loop:

	alu.and t4, t3, nil, 0xff

	jmp.beq nil, t4, zero, @strlen_end
	alu.srl t3, t3, #8


	alu.sub t1, t1, #1
	alu.add t2, t2, #1

	jmp.bne nil, t1, zero, @strlen_char_loop

	alu.add t0, t0, #1
	jmp.jmp nil, nil, nil, @strlen_loop


strlen_end:
	alu.add a0, t2, zero
	jmp.ret nil, nil, nil
;---------------------------------

;a0 = src1
;a1 = src2
strcpy:
;TODO
	alu.add t0, a0, zero
	alu.add t1, a1, zero


strcpy_loop:

	jmp.jmp nil, nil, nil, @strcpy_loop

strcpy_end:


	jmp.ret nil, nil, nil
;---------------------------------

;a0 = dst1
;a1 = src2
strcmp:
;TODO
	alu.add t0, a0, zero
	alu.add t1, a1, zero


	;counter
	alu.add t2, zero, zero
strcmp_loop:

	mem.ld t3, t1, zero

strcpy_check:




strcmp_end:
	jmp.ret nil, nil, nil
;---------------------------------

;a0 = src1
;a1 = src2
;a2 = length
strncpy:
;TODO
;---------------------------------

;a0 = source
;a1 = index
strindex:

	alu.add t0, a0, zero
	alu.add t1, a1, zero
	alu.and t2, t1, #7
	alu.srl t3, t1, #3
	mem.ld a0, a0, t3
	alu.sll t2, t2, #3
	alu.srl a0, a0, t2
	alu.and a0, a0, nil, 0xff
	jmp.ret nil, nil, nil

;a0 = source
;a1 = index
;a2 = value
setindex:
	mem.push nil, s0, zero
	mem.push nil, s1, zero
	mem.push nil, s2, zero
	mem.push nil, s3, zero
	alu.srl s2, a0, #3
	mem.ld s0, a0, s2
	alu.and s1, a1, #7
	alu.and a2, a2, nil, 0xff
	alu.sll s3, s1, #3

;---------------------------------

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
	jmp.bne nil, t2, zero, @memcpy_loop
	jmp.ret zero, zero, zero



;---------------------------------


;a0 = source
;a1 = value
;a2 = length
memset:
	alu.add t0, zero, zero

memset_loop:
	mem.st! a1, a0, t0
	alu.add t0, t0, #1
	jmp.blt nil, t0, a2, @memset_loop

	jmp.ret zero, zero, zero



;a0 src
reverse_word:
	alu.sll a1, a0, #0
	alu.sll a2, a0, #8
	alu.sll a3, a0, #16
	alu.sll a4, a0, #24
	alu.sll a5, a0, #32
	alu.sll a6, a0, #40
	alu.sll a7, a0, #48
	alu.sll a8, a0, #56

	alu.add t0, nil, nil, 0xff

	alu.and a1, a1, t0
	alu.and a2, a2, t0
	alu.and a3, a3, t0
	alu.and a4, a4, t0
	alu.and a5, a5, t0
	alu.and a6, a6, t0
	alu.and a7, a7, t0
	alu.and a8, a8, t0

;---------------------------------
;[abcdef]
;a0 src
reverse:
	mem.push nil, a0, nil
	jmp.call nil, nil, nil, @strlen
	alu.add t1, a0, zero
	mem.pop t0, zero, zero
	;align end to ptr
	;len >> 3 = words size
	alu.srl t2, t1, #3
	;remaining bytes masked &0x3
	alu.and t3, t1, #7
	;t4 = len(words) + address base


_reverse_step_down:
	mem.ld t4, t0, t2

	mem.ld t5, t1, zero



	jmp.ret zero, zero, zero



