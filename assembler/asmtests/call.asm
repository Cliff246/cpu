.text


MAIN:
	mem.sp x0, x0, x0, 1000
	mem.sfp x0, x0, x0, 1000

	jmp.call x0, x0, x0, @TARGET_FUNC

	jmp.jmp x0, x0, x0, @END

TARGET_FUNC:

	alu.add x2, x2, x0, 2
	jmp.ret x0, x0, x0


END:
	jmp.jmp x0, x0, x0, @END

