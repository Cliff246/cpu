

;
%def SORT



.text tag std 

;a0 dest
;a1 source
;a2 size
;a3 function
SORT:
	

;setting saves 
	alu.add s0, a0, zero
	alu.add s1, a1, zero
	alu.add s2, a2, zero
	alu.add s3, a3, zero
	
		

	jmp.call null, null, null, s3


	
