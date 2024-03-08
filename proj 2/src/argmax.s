.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	addi t1 x0 1
	blt a1 t1 length_error
	addi t2 x0 1
    addi t3 x0 4
    sub t1 t1 t1
    add t6 x0 x0
	lw t0 0(a0)
    
loop_start:
	beq t2 a1 loop_end
    mul t4 t2 t3
    add t5 a0 t4
    lw t6 0(t5)
    blt t6 t0 loop_continue
    mv t1 t2
    mv t0 t6
    j loop_continue
        
loop_continue:
	addi t2 t2 1
    j loop_start

length_error:
	li a0 36
    j exit

loop_end:
	mv a0 t1
	ret
