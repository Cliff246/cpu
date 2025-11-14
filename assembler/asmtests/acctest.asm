


.text


start:
	alu.add t1, zero, #20
	alu.add t0, zero, zero, 1000
	alu.add acc, zero, #5
	alu.add! t2, t0, t1, acc



	alu.add acc, acc, #10
