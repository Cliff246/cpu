	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 2
	.globl	_multiply_matrix                ; -- Begin function multiply_matrix
	.p2align	2
_multiply_matrix:                       ; @multiply_matrix
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	str	x0, [sp, #40]
	str	x1, [sp, #32]
	str	x2, [sp, #24]
	str	w3, [sp, #20]
	str	wzr, [sp, #16]
	b	LBB0_1
LBB0_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_3 Depth 2
                                        ;       Child Loop BB0_5 Depth 3
	ldr	w8, [sp, #16]
	ldr	w9, [sp, #20]
	subs	w8, w8, w9
	b.ge	LBB0_12
	b	LBB0_2
LBB0_2:                                 ;   in Loop: Header=BB0_1 Depth=1
	ldr	w8, [sp, #16]
	ldr	w9, [sp, #20]
	mul	w8, w8, w9
	str	w8, [sp, #12]
	str	wzr, [sp, #8]
	b	LBB0_3
LBB0_3:                                 ;   Parent Loop BB0_1 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_5 Depth 3
	ldr	w8, [sp, #8]
	ldr	w9, [sp, #20]
	subs	w8, w8, w9
	b.ge	LBB0_10
	b	LBB0_4
LBB0_4:                                 ;   in Loop: Header=BB0_3 Depth=2
	str	wzr, [sp, #4]
	b	LBB0_5
LBB0_5:                                 ;   Parent Loop BB0_1 Depth=1
                                        ;     Parent Loop BB0_3 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	w8, [sp, #4]
	ldr	w9, [sp, #20]
	subs	w8, w8, w9
	b.ge	LBB0_8
	b	LBB0_6
LBB0_6:                                 ;   in Loop: Header=BB0_5 Depth=3
	ldr	x8, [sp, #40]
	ldr	w9, [sp, #12]
	ldr	w10, [sp, #4]
	add	w9, w9, w10
	ldr	w8, [x8, w9, sxtw #2]
	ldr	x9, [sp, #32]
	ldr	w10, [sp, #4]
	ldr	w11, [sp, #20]
	mul	w10, w10, w11
	ldr	w11, [sp, #8]
	add	w10, w10, w11
	ldr	w9, [x9, w10, sxtw #2]
	mul	w11, w8, w9
	ldr	x9, [sp, #24]
	ldr	w8, [sp, #12]
	ldr	w10, [sp, #8]
	add	w10, w8, w10
	ldr	w8, [x9, w10, sxtw #2]
	add	w8, w8, w11
	str	w8, [x9, w10, sxtw #2]
	b	LBB0_7
LBB0_7:                                 ;   in Loop: Header=BB0_5 Depth=3
	ldr	w8, [sp, #4]
	add	w8, w8, #1
	str	w8, [sp, #4]
	b	LBB0_5
LBB0_8:                                 ;   in Loop: Header=BB0_3 Depth=2
	b	LBB0_9
LBB0_9:                                 ;   in Loop: Header=BB0_3 Depth=2
	ldr	w8, [sp, #8]
	add	w8, w8, #1
	str	w8, [sp, #8]
	b	LBB0_3
LBB0_10:                                ;   in Loop: Header=BB0_1 Depth=1
	b	LBB0_11
LBB0_11:                                ;   in Loop: Header=BB0_1 Depth=1
	ldr	w8, [sp, #16]
	add	w8, w8, #1
	str	w8, [sp, #16]
	b	LBB0_1
LBB0_12:
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	wzr, [x29, #-4]
	stur	w0, [x29, #-8]
	str	x1, [sp, #16]
	adrp	x0, _matrix1@PAGE
	add	x0, x0, _matrix1@PAGEOFF
	adrp	x1, _matrix2@PAGE
	add	x1, x1, _matrix2@PAGEOFF
	adrp	x2, _matrix3@PAGE
	add	x2, x2, _matrix3@PAGEOFF
	mov	w3, #3                          ; =0x3
	bl	_multiply_matrix
	str	wzr, [sp, #12]
	b	LBB1_1
LBB1_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB1_3 Depth 2
	ldr	w8, [sp, #12]
	subs	w8, w8, #3
	b.ge	LBB1_8
	b	LBB1_2
LBB1_2:                                 ;   in Loop: Header=BB1_1 Depth=1
	str	wzr, [sp, #8]
	b	LBB1_3
LBB1_3:                                 ;   Parent Loop BB1_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	w8, [sp, #8]
	subs	w8, w8, #3
	b.ge	LBB1_6
	b	LBB1_4
LBB1_4:                                 ;   in Loop: Header=BB1_3 Depth=2
	ldr	w8, [sp, #12]
	mov	w9, #3                          ; =0x3
	mul	w8, w8, w9
	ldr	w9, [sp, #8]
	add	w9, w8, w9
	adrp	x8, _matrix3@PAGE
	add	x8, x8, _matrix3@PAGEOFF
	ldr	w8, [x8, w9, sxtw #2]
                                        ; kill: def $x8 killed $w8
	mov	x9, sp
	str	x8, [x9]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_printf
	b	LBB1_5
LBB1_5:                                 ;   in Loop: Header=BB1_3 Depth=2
	ldr	w8, [sp, #8]
	add	w8, w8, #1
	str	w8, [sp, #8]
	b	LBB1_3
LBB1_6:                                 ;   in Loop: Header=BB1_1 Depth=1
	b	LBB1_7
LBB1_7:                                 ;   in Loop: Header=BB1_1 Depth=1
	ldr	w8, [sp, #12]
	add	w8, w8, #1
	str	w8, [sp, #12]
	b	LBB1_1
LBB1_8:
	ldur	w0, [x29, #-4]
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__DATA,__data
	.globl	_matrix1                        ; @matrix1
	.p2align	2, 0x0
_matrix1:
	.long	1                               ; 0x1
	.long	2                               ; 0x2
	.long	3                               ; 0x3
	.long	4                               ; 0x4
	.long	5                               ; 0x5
	.long	6                               ; 0x6
	.long	7                               ; 0x7
	.long	8                               ; 0x8
	.long	9                               ; 0x9

	.globl	_matrix2                        ; @matrix2
	.p2align	2, 0x0
_matrix2:
	.long	4294967293                      ; 0xfffffffd
	.long	4294967292                      ; 0xfffffffc
	.long	4294967291                      ; 0xfffffffb
	.long	4294967286                      ; 0xfffffff6
	.long	4294967288                      ; 0xfffffff8
	.long	6                               ; 0x6
	.long	12                              ; 0xc
	.long	4294967284                      ; 0xfffffff4
	.long	20                              ; 0x14

	.globl	_matrix3                        ; @matrix3
.zerofill __DATA,__common,_matrix3,36,2
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%d\n"

.subsections_via_symbols
