


.text tag std start


start:
	mem.sp null, null, null, 1000
	mem.sfp null, null, null, 1000

	alu.add a0, zero, zero, vec1
	alu.add a1, zero, zero, vec2
	alu.add a2, zero, #3

	jmp.call null, null, null, @dotproduct
end:
	jmp.jmp null, null, null, @end

;a0 = address vec1
;a1 = address vec2
;a2 = len
dotproduct:

	mem.push null, a0, null
	mem.push null, a1, null
	mem.push null, a2, null


	alu.add t0, zero, zero
	alu.add t1, zero, zero
	alu.add t2, zero, zero
dotloop:
	;
	mem.ld t3, a0, t0
	mem.ld t4, a1, t0
	;a * b
	alu.mul t5, t3, t4
	;ab +
	alu.add t1, t1, t5
	alu.add t0, t0, #1

	jmp.blt null, t0, a2, @dotloop


	mem.pop a2, null, null
	mem.pop a1, null, null
	alu.add a0, t1, zero

	jmp.ret null, null, null


.data


vec1:
	i64 1 3, -5
vec2:
	i64 1, 3, -5