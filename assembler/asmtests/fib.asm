



.text tag fib start


START:

	mem.sp zero, zero, zero, 1000
	mem.sfp zero, zero, zero, 1000


	alu.add a0, zero, zero, 10
	alu.add a1, zero, zero, 0

	jmp.call null, null, null, @FIB
DONE:
	jmp.jmp null, null, null, @DONE

;a0 = limit
;a1 = f0
;a2 = f1
FIB:

	jmp.ble null, a0, #1, @ESCAPE

	alu.sub a1, a0, #1
	jmp.call null, null, null, @FIB
	mem.push zero, a0, zero

	mem.pop t0, null, null

	alu.sub a1, a0, #2
	jmp.call null, null, null, @FIB
	alu.add a0, a0, t0

ESCAPE:

	jmp.ret null, null, null


