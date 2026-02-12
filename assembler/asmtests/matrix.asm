


.text tag std


start:
	mem.sp nil, nil, nil, 1000
	mem.sfp nil, nil, nil, 1000

	alu.add a0, zero, zero, mat1
	alu.add a1, zero, zero, mat2
	alu.add a2, zero, zero, mat3
	alu.add a3, zero, zero, 3


	jmp.call nil, nil, nil, @mat_mul

end:
	jmp.jmp nil, nil, nil, @end

;a0 ptr2
;a1 ptr1
;a2 dest
;a3 mat size
mat_mul:
	;push spots
	mem.pushw! a0, zero, zero
	mem.pushw! a1, zero, zero
	mem.pushw! a2, zero, zero
	mem.pushw! a3, zero, zero
	alu.add t10, zero, zero



	;tix = iterator x
	alu.add t17, zero, zero

	;t0	= x * size
	;rx -> t0
	alu.add t0, zero, zero
mat_x:
	;tiy = iterator y
	alu.add t18, zero, zero
mat_y:
	;tiz = iterator z
	alu.add t19, zero, zero



mat_z:

	;t1 = rx + tiz
	alu.add t1, t0, t19
	mem.ldw t3, t1, a0


	;b[(tiz * size) + tiy)
	alu.mul t2, t19, a3
	alu.add t4, t2, t18
	mem.ldw t5, t4, a1

	sys.break nil, nil, nil


	;dot product result
	;dot = t5 * t3
	alu.mul t7, t5, t3
	;t8 = rx + y
	alu.add t8, t0, t18

	;t9 = a2[rx + tiy]
	mem.ldw t9, t8, a2
	;t9 = t9 + dot
	alu.add t9, t9, t7
	;a2[t0 + tiy]
	mem.stw! t9, t8, a2


	;tiz += 1
	alu.add t19, t19, #1

	;loop 1
	alu.add t10, t10, #1
	sys.break nil, nil, nil
	jmp.blt zero, t19, a3, @mat_z


	;tiy += 1
	alu.add t18, t18, #1

	;loop 2
	jmp.blt zero, t18, a3, @mat_y
	;rx += size
	alu.add t0, t0, a3

	alu.add t17, t17, #1

	jmp.blt zero, t17, a3, @mat_x









mat_ret:

	mem.popw a3, zero, zero
	mem.popw a2, zero, zero
	mem.popw a1, zero, zero
	mem.popw a0, zero, zero


	jmp.ret zero, zero, zero

.data tag matrix_temp


mat1:
	;row 1
	i64 1, 2, 3
	i64 4, 5, 6
	i64 7, 8, 9


mat2:
	i64 6, -9, 12
	i64	3, 0, 15
	i64 21, -12, 18


mat3:
	mem 9

; 55 0x0000007e 126
;  56 0x00000093 147
;  57 0x000000a8 168
;  58 0x0000007e 126
;  59 0x00000093 147
;  60 0x000000a8 168
;  61 0x0000007e 126
;  62 0x00000093 147
;  63 0x000000a8 168
