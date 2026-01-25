
%imp "stl/string.asm"
%imp "stl/sio.asm"
%imp "stl/jumptable_stl.asm"

%def sio_print
%def stl
%order image std

.text tag image start


MAIN:

	mem.sp nil, nil, nil, 1000
	mem.sfp nil, nil, nil, 1000
	alu.add a1, zero, zero, TEST_STRING
	alu.add a2, zero, zero, 9
	alu.add a0, zero, zero, @sio_print
	sys.setcd nil, nil, nil, stl


.data

TEST_STRING:
	string "hello world"
