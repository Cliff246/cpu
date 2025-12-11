
%pub MAIN LOOP BYTE_SPLIT




.text

MAIN:
	; t1 = pointer
	; t2 = counter
	alu.add t1, zero, zero           ; start pointer = 0
	alu.add t2, zero, zero           ; length = 0

LOOP:
	; t3 = data
	mem.ld t3, zero, t1, str        ; load one word
	; t4 = count of 8
	alu.add t4, zero, #8
BYTE_SPLIT:
	;x2 = mask
	alu.and t5, t3, null, 0xFF      ; mask lowest byte (flag=1)
	jmp.beq null, t5, zero, @END      ; stop at null terminator
	alu.srl t3, t3, #8

	;x6 = x6 + 1
	alu.add t2, t2, #1        ; length++
	;x4 = x4 - 1
	alu.sub t4, t4, #1        ; move to next byte
	;jmp on x4 != 0
	jmp.bne null, t4, zero, @BYTE_SPLIT

	alu.add t1, t1, #1

	jmp.jmp null, null, null, @LOOP
END:
	jmp.jmp null, null, null, @END      ; halt here

.data
str:
	string "abc"

