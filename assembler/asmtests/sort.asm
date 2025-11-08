

;
%pub SORT



.text tag std

START:
	mem.sp zero, zero, zero, 1000
	alu.add a0, zero, zero, ARRY
	alu.add a1, zero, zero
	alu.add a2, zero, #5

	jmp.call null, null, null, @SORT


;exs0 desc dest
;exs1 jmp dest
;exs2 arg 0
;exs3 arg 1
JUMP_TO_DEST:
	;move a0 and a1 to sx2 and sx3
	alu.add a0, exs2, zero
	alu.add a1, exs3, zero
	;fake call outside of description
	sys.dcall null, exs0, exs1
	;move actual return to external return
	alu.add exs0, a0, zero
	;return out of jump function
	jmp.ret null, null, null

;a0 = arr
;a1 = low
;a2 = high
PARTITION:

	;t0 = arr[low]
	mem.ld t0, a0, a1
	;t1 = low
	alu.add t1, a1, zero
	;t2 = high
	alu.add t2, a2, zero

PARTITION_WHILE_1:
	;t5 = high - 1
	alu.sub t7, t2, #1
	;t6 = low + 1
	alu.add t8, t1, #1
PARTITION_GREATER:
	;t1 = t1 + 1
	mem.ld t3, a0, t1
	;arr[t3] <= t0
	jmp.ble null, t3, t0, @PARTITION_LESS
	jmp.ble null, t1, t7, @PARTITION_LESS
	alu.add t1, t1, #1
	jmp.jmp null, null, null, @PARTITION_GREATER
PARTITION_LESS:


	mem.ld t3, a0, t2
	jmp.blt null, t0, t3, @SWAP
	jmp.blt null, t0, t3, @SWAP

	alu.sub t2, t2, #1
	jmp.jmp null, null, null, @PARTITION_LESS


SWAP:
	jmp.ble null, t1, t2, @ELSE_SWAP

	alu.add t9, a0, t1
	alu.add t10, a0, t2
	mem.ld t4, t9, zero
	mem.ld t5, t10, zero

	mem.sd null, t5, t9
	mem.sd null, t4, t10
ELSE_SWAP:



	jmp.bltu null, t1, t2, @PARTITION_WHILE_1





;a0 source
;a1 low
;a2 hi
SORT:


;setting saves
	;array
	mem.push zero, a0, zero
	;low
	mem.push zero, a1, zero
	;high
	mem.push zero, a2, zero

	jmp.blt null, a1, a2, @SORT_DOWN


	;arr, low, high
	jmp.call null, null, null, @PARTITION


	;pi = s0
	alu.add s0, a0, zero

	alu.sub a2, s0, #1
	;add a2 to stack ptr stack at four
 	mem.push null, a2, null
	jmp.call null, null, null, @SORT
	;get back a2 stack at four to three
	mem.pop t0, null, null
	;stack is at 3
	alu.add a1, t0, #1
	;stack is at 2
	;descrement stack ptr
	mem.pop a2, null, null
	;decrement back to 0
	mem.lds a0, null, #2
	;second jump
	jmp.call null, null, null, @SORT
SORT_DOWN:
	;third jump
	jmp.ret null, null, null





.data

ARRY:
	i64 4, 2, 5, 3, 1


