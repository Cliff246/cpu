%imp "stl/string.asm"
%def memcpy
%def memset
%pub sio_print
.text tag std

;a0 is ptr
;a1 is length
sio_print:

	;redundant for now
	alu.add s0, a0, zero
	alu.add s1, a1, zero
	mem.push null, s0, null
	mem.push null, s1, null

	mem.ld t0, zero, zero, SIO_OUTBUFFER_ADDRESS
	mem.ld t1, zero, zero, SIO_OUTBUFFER_LENGTH

	alu.add a0, t0, zero
	alu.add a1, t1, zero
	jmp.call zero, zero, zero, @memset

	mem.ld a0, zero, zero, SIO_OUTBUFFER_ADDRESS
	mem.ld a2, zero, zero, SIO_OUTBUFFER_LENGTH

	mem.pop s1, null, null
	;put in jmp.call's src slot not length slot
	mem.pop a1, null, null

	jmp.call zero, zero, zero, @memcpy

	mem.ld t0, zero, zero, SIO_PRESENT_ADDRESS
	alu.add t1, zero, #1
	mem.st! t1, t0, zero
	jmp.ret null, null, null

.data tag std_extra


SIO_OUTBUFFER_ADDRESS:
	i64 10000
SIO_OUTBUFFER_LENGTH:
	i64 100
SIO_PRESENT_ADDRESS:
	i64 10101