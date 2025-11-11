

%imp "copy.asm"
%order std dat
%def COPY

.text tag std start

MAIN:
	mem.sp null, null, null, 1000
	mem.sfp null, null, null, 1000

	alu.add a0, zero, zero, dest
	alu.add a1, zero, zero, source
	alu.add a2, zero, #3
	jmp.call null, null, null, @COPY


.data tag dat

source:
	string "hello small world"
dest:
	mem 100