.text


MAIN:
	mem.sp zero, zero, zero, 1000
	mem.sfp zero, zero, zero, 1000

	jmp.call zero, zero, zero, @QUADRATIC
RESET:
	jmp.jmp zero, zero, zero, @RESET
	

;takes sqrt as x8
;a0 = y
SQRT:
	;N = a0
	;x(t0) = a0
	alu.add t0, a0, zero
	;y(t1) = 1
	alu.add t1, zero, zero, 1
	;iter(t4) = 10
	alu.add t4, zero, zero, 10

	;epsion
	alu.add g0, zero, zero, 10

INNER_SQRT
	;we need to do k' = (k + (y/k))/2
	;diff = x - y
	alu.sub t2, t0, t1
	;escape
	;branch if diff < epsilon
	jmp.blt null, t2, g0, @DONE
	;temp = x + y
	alu.add t3, t0, t1
	;x = temp / 2
	alu.srl t0, t3, null, 1
	; y = n / x
	alu.div t1, a0, t0



	;t4 = t4 - 0 - 1
	alu.sub t4, t4, zero, 1


	;jump if x10 != 0
	jmp.bne null, t4, zero, @INNER_SQRT
	;set a1
DONE:
	alu.add a1, t0, zero
	jmp.ret null, null, null
;x1 = A
;x2 = B
;x3 = C
QUADRATIC:

	alu.add a0, zero, zero, 1000

	jmp.call zero, zero, zero, @SQRT



	jmp.ret x0, x0, x0

