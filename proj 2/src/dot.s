.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
	addi t0 x0 1
	blt a3 t0 strideerror
    blt a4 t0 strideerror
	blt a2 t0 lenerror
    addi t0 t0 3
    add t6 x0 x0
    add t1 x0 x0
    
loop_start:
	beq t1 a2 loop_end
    mul t2 a3 t0
    mul t2 t2 t1
    add t3 a0 t2
    lw t4 0(t3)
    mul t2 a4 t0
    mul t2 t2 t1
    add t3 a1 t2
    lw t5 0(t3)
    mul t4 t4 t5
    add t6 t6 t4
	j loop_continue
    
loop_continue:
	addi t1 t1 1
	j loop_start

strideerror:
	li a0 37
    j exit

lenerror:
	li a0 36
    j exit

loop_end:
	mv a0 t6
    
	ret
