%imp "stl/string.asm"
%def strcpy
%pub sio_print

.text tag std


sio_print:



	jmp.call zero, zero, zero, @strcpy

.data tag std_extra


SIO_OUTBUFFER_ADDRESS:
	i64 10000
SIO_OUTBUFFER_LENGTH:
	i64 1000
