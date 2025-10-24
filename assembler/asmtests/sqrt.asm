

.text


	;takes a a0 as an argument
SQRT_FUNCTION:

	alu.add a0, zero, zero, 10

	; op = t0
	alu.add t0, a0, zero
	;result = t1
	alu.add t1, zero, zero
	;one = 1
	alu.add t2, zero, zero, 1

WHILE_1:
	;one <<= 2
	alu.sll t2, t2, null, 2
	jmp.ble null, t2, a0, @WHILE_1


	;one is the larget power four <= x
	alu.sra t2, t2, null, 2

WHILE_2:

	;t3 = res + one
	alu.add t3, t1, t2
	;op >= res + one
	;res + one < op
	jmp.bltu null, t0, t3, @ELSE
	;op -= res + one
	alu.sub t0, t0, t3

	;res += 2 * one
	alu.sll t4, t2, null, 2
	alu.add t1, t1, t4



ELSE:

	;res //= 2
	alu.sra t1, t1, null, 2
	;one //= 4
	alu.sra t2, t2, null, 4


	jmp.bne null, t2, zero, @WHILE_2



;start
