.text

MAIN:
	; x5 = pointer
	; x6 = counter
	alu.add x5, x0, x0           ; start pointer = 0
	alu.add x6, x0, x0           ; length = 0

LOOP:
	; x1 = data
	mem.ld x1, x0, x5, str        ; load one word
	; x4 = count of 8
	alu.add x4, x0, x0, 8
BYTE_SPLIT:
	;x2 = mask
	;
	alu.and x2, x1, x0, 0xFF      ; mask lowest byte (flag=1)
	jmp.beq x0, x2, x0, @END      ; stop at null terminator
	alu.srl x1, x1, x0, 8

	;x6 = x6 + 1
	alu.add x6, x6, x0, 1         ; length++
	;x4 = x4 - 1
	alu.sub x4, x4, x0, 1         ; move to next byte
	;jmp on x4 != 0
	jmp.bne x0, x4, x0, @BYTE_SPLIT

	alu.add x5, x5, x0, 1

	jmp.jmp x0, x0, x0, @LOOP
END:
	jmp.jmp x0, x0, x0, @END      ; halt here

.data
str:
	STRING "hello world this is"
