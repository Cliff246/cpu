

;
%pub SORT



.text tag std

START:
	mem.sp zero, zero, zero, 1000
	mem.sfp zero, zero, zero, 1000
	alu.add g0, zero, zero, ARRY
	alu.add a0, zero, zero
	alu.add a1, zero, #5
	jmp.call null, null, null, @SORT

EXIT:
	jmp.jmp null, null, null, @EXIT

;a0 = low
;a1 = high
;g0 = arry
PARTITION:



	;load arry[high]
	mem.ld t0, a1, g0
	;i = a0 - 1
	alu.sub t1, a0, #1

	;t2 = j
	alu.add t2, a0, zero
	;high - 1
	alu.sub t3, a1, #1


PARTITION_WHILE_1:


	;array[j] = t4
	mem.ld t4, t2, g0
	;t4 < t0
	jmp.ble null, t4, t0, @PARTITION_END
	;i += 1
	alu.add t1, t1, #1
	;get the offsets
	alu.add t8, t1, g0
	alu.add t9, t2, g0
	;swap the two
	mem.ld t5, t1, g0

	mem.ld t6, t2, g0

	mem.st! t5, t2, g0
	mem.st! t6, t1, g0

PARTITION_END:
	alu.add t2, t2, #1

	jmp.ble null, t2, t3, @PARTITION_WHILE_1

	alu.add t1, t1, #1
	;i + 1 spot
	alu.add t12, t1, g0
	;high spot
	alu.add t13, a1, g0


	mem.ld t10, t12, zero
	mem.ld t11, t13, zero


	mem.st null, t10, t13
	mem.st null, t11, t12
	;set g0
	alu.add g1, t1, zero




	jmp.ret null, null, null


;low = a0
;high = a1
SORT:


	;push
	mem.push null, a0, null
	mem.push null, a1, null

	jmp.blt null, a0, a1, @SORT_CONTINUE
	jmp.jmp null, null, null, @SORT_DOWN
SORT_CONTINUE:
	jmp.call null, null, null, @PARTITION
	; call left side
	mem.push null, g1, null
	alu.sub a1, g1, #1
	jmp.call null, null, null, @SORT
	mem.pop g1, null, null

	; call right side
	alu.add a0, g1, #1
	jmp.call null, null, null, @SORT

SORT_DOWN:
	mem.pop a1, null, null
	mem.pop a0, null, null


	jmp.ret null, null, null
.data

ARRY:
	i64 10, 7, 8, 9, 1, 5

