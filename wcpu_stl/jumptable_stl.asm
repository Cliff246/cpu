
%imp "stl/sio.asm"
%imp "stl/string.asm"

%def memcpy
%def memset
%def sio_print
%pub stl

%pub jumptable

.text tag std ref stl start


;jmp to function in stl
jumptable:

	;set the t0
	alu.add t0, a0, zero
	sys.break nil, nil, nil
	alu.add a0, a1, zero
	alu.add a1, a2, zero
	alu.add a2, a3, zero
	jmp.call nil, nil, t0

repeat:
	jmp.jmp nil, nil, nil, @repeat
