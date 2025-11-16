%order std program

.text tag std ref hello_world


start:

	alu.add a0, zero, zero, 123456789


.data

value:
	string "the little white rabbit jumped over the hen"

.text tag program start
main:
	alu.add x0, x0, x0, hello_world

