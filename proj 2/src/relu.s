.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	# Prologue
    addi t1 x0 1
	blt a1 t1 length_error
	sub t2 x0 x0
    addi t3 x0 4

loop_start:
	beq t2 a1 loop_end
    mul t4 t2 t3
    add t5 a0 t4
    lw t6 0(t5)
    blt t6 x0 set
    j loop_continue
    
set:
	mul t6 t6 x0
    sw t6 0(t5)
    j loop_continue

loop_continue:
	addi t2 t2 1
    j loop_start

length_error:
	li a0 36
    j exit

loop_end:
	ret
