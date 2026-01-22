

%pub SQRT
%pub POW
%pub LOG2
%pub LOG10
%pub MATADD
%pub MATMUL
%pub DOTPRODUCT
%pub MATSCALE
%pub MEAN
%pub STDDEV
.text tag std

;TODO
;---------------------------

SQRT:






	jmp.ret null, null, null


;---------------------------
;TODO
POW:

	jmp.ret zero, zero, zero


;---------------------------


;takes a0 as N
LOG2:
	alu.add t0, zero, zero
LOOP_LOG2:
	alu.srl a0, a0, #1
	alu.add t0, t0, #1
	jmp.ble null, a0, zero, @LOOP_LOG2

	alu.add a0, t0, zero
	jmp.ret null, null, null

;---------------------------

;TODO
LOG10:
	mem.push null, s0, null
	alu.add s0, a0, zero
	;returns on a0
	jmp.call null, null, null, @LOG2
	;load 1 into lhs
	alu.add t1, zero, #1
	;computer 1ull << a0
	alu.sll t0, t1, a0
	;t0 != s0
	jmp.beq null, t0, s0, @ELSE_LOG10
	alu.add a0, a0, #1
ELSE_LOG10:
	mem.pop s0, null, null


;---------------------------
;TODO
;a0 list
;a1 length
MEAN:
	mem.push null, acc, null
	mem.push null, s0, null
	alu.add acc, zero, zero
	alu.add s0, a0, zero
MEAN_L1:
	mem.ld t0, a0, s0
	alu.add acc, acc, t0
	alu.sub s0, s0, #1

	jmp.bne null, s0, zero, @MEAN_L1

;---------------------------
	alu.div a0, acc, a1
	mem.pop s0, zero, zero
	mem.pop acc, zero, zero
	jmp.ret zero, zero, zero

;a0 list
;a1 length
STDDEV:
	alu.add s0, a0, zero
	alu.add s1, a1, zero




	jmp.call zero, zero, zero, @MEAN
	alu.add s2, a0, zero

	alu.add t0, zero, zero


STDDEV_L1:

	;ld elem
	mem.ld t1, s0, t0
	;elem - mean
	alu.sub t1, t1, s2
	;square inner
	alu.mul! t1, t1, acc
	alu.add t0, t0, #1
	jmp.bne null, s1, t0, @STDDEV_L1
	alu.div s3, acc, s1
	alu.add a0, s3, zero
	jmp.call zero, zero, zero, @SQRT
	jmp.ret zero, zero, zero
;---------------------------

;TODO

DOTPRODUCT:


	jmp.ret zero, zero, zero


;---------------------------
;TODO
;a0 dst
;a1 src1
;a2 src2
;a3 size N
VECADD:




	jmp.ret zero, zero, zero



;---------------------------
;TODO
VECNORM:

	jmp.ret zero, zero, zero

;---------------------------
;TODO

;a0 matrix dst
;a1 matrix src;
;a2 scaler
;a3 matrix size M
;a3 matrix size N
MATSCALE:


	jmp.ret zero, zero, zero

;---------------------------

;TODO
;a0 matrix dst
;a1 matrix src 1
;a2 matrix src 2
;a3 matrix size M
;a4 matrix size N
MATADD:



	jmp.ret zero, zero, zero


;---------------------------
;transpose
;a0 dst
;a1 src
;a2 size M
;a3 size N

MATTRANSPOSE:

	jmp.ret zero, zero, zero

;---------------------------

;a0 src,
;a1 dst

MATGAUS:
	jmp.ret zero, zero, zero



;---------------------------

;---------------------------

;returns pos on complete, neg on fail
;a0 matrix dst
;a1	matrix src 1
;a2 matrix src 2
;a3	matrix src 1 M
;a4 matrix src 1 N
;a5 matrix src 2 M
;a6 matrix src 2 N

MATMUL:
	alu.add s0, a0, zero
	alu.add s1, a1, zero
	alu.add s2, a2, zero
	alu.add s3, a3, zero
	alu.add s4, a4, zero
	alu.add s5, a5, zero
	alu.add s6, a6, zero


;check valid shape




MATMUL_ROW:


MATMUL_COL:


MATMUL_DOT:


	jmp.jmp null, null, null, @MATMUL_DONE

MATMUL_DONE:

	alu.add a0, zero, #1
	jmp.ret zero, zero, zero
MATMUL_FAIL:

	alu.sub a0, zero, #1
	jmp.ret zero, zero, zero
;---------------------------



;a0->a8 return
;t0->t8 mat1
;t9->t17 mat2
MATMUL3x3_INLINE:

	;col 1
	alu.mul acc, t0, t9
	alu.mul! t1, t12, acc
	alu.mul! t2, t15, acc
	alu.add a0, zero, acc

	alu.mul acc, t0, t10
	alu.mul! t1, t13, acc
	alu.mul! t2, t16, acc
	alu.add a1, zero, acc

	alu.mul acc, t0, t11
	alu.mul! t1, t14, acc
	alu.mul! t2, t17, acc
	alu.add a2, zero, acc

	;col 2
	alu.mul acc, t3, t9
	alu.mul! t4, t12, acc
	alu.mul! t5, t15, acc
	alu.add a3, zero, acc

	alu.mul acc, t3, t10
	alu.mul! t4, t13, acc
	alu.mul! t5, t16, acc
	alu.add a4, zero, acc

	alu.mul acc, t3, t11
	alu.mul! t4, t14, acc
	alu.mul! t5, t17, acc
	alu.add a5, zero, acc

	;col 3
	alu.mul acc, t6, t9
	alu.mul! t7, t12, acc
	alu.mul! t8, t15, acc
	alu.add a6, zero, acc

	alu.mul acc, t6, t10
	alu.mul! t7, t13, acc
	alu.mul! t8, t16, acc
	alu.add a7, zero, acc

	alu.mul acc, t6, t11
	alu.mul! t7, t14, acc
	alu.mul! t8, t17, acc
	alu.add a8, zero, acc

	jmp.ret zero, zero, zero