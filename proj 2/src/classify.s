.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
	addi sp sp -68
    sw s0 8(sp)
    sw s1 12(sp)
    sw s2 16(sp)
    sw s3 20(sp)
    sw s4 24(sp)
    sw s5 28(sp)
    sw s6 32(sp)
    sw s7 36(sp)
    sw s8 40(sp)
    sw s9 44(sp)
    sw s10 48(sp)
    sw s11 52(sp)
    sw ra 56(sp)
    
    li t0 5
    bne t0 a0 arg_err
    
    mv s0 a0
    mv s1 a1
    mv s2 a2
    
    #create space for row/col of M0 and read it
    li a0 8
    jal malloc
    beq a0 x0 malloc_err
    mv s3 a0
    addi a0 a0 4
    mv s4 a0
    lw a0 4(s1)
    mv a1 s3
    mv a2 s4
    jal read_matrix
    mv s5 a0
    
    #create space for row/col of M1 and read it
    li a0 8
    jal malloc
    beq a0 x0 malloc_err
    mv s6 a0
    addi a0 a0 4
    mv s7 a0
    lw a0 8(s1)
    mv a1 s6
    mv a2 s7
    jal read_matrix
    mv s8 a0
    
    #create space for row/col of input and read it
	li a0 8
    jal malloc
    beq a0 x0 malloc_err
    mv s9 a0
    addi a0 a0 4
    mv s10 a0
    lw a0 12(s1)
    mv a1 s9
    mv a2 s10
    jal read_matrix
    mv s11 a0
    
	# Compute h = matmul(m0, input)
	lw t0 0(s3)
    lw t1 0(s10)
    mul t0 t0 t1
    sw t0 0(sp)
    li t1 4
    mul t0 t0 t1
    mv a0 t0
    jal malloc
    beq a0 x0 malloc_err
    sw a0 4(sp)
    
    mv a0 s5
    lw a1 0(s3)
    lw a2 0(s4)
    mv a3 s11
    lw a4 0(s9)
    lw a5 0(s10)
    lw a6 4(sp)
    jal matmul
    

	# Compute h = relu(h)
	lw a0 4(sp)
    lw a1 0(sp)
    jal relu
    

	# Compute o = matmul(m1, h)
	lw t0 0(s6)
    lw t1 0(s10)
    mul t0 t0 t1
    sw t0 60(sp)
    li t1 4
    mul t0 t0 t1
    mv a0 t0
    jal malloc
    beq a0 x0 malloc_err
    sw a0 64(sp)
    
    mv a0 s8
    lw a1 0(s6)
    lw a2 0(s7)
	lw a3 4(sp)
    lw a4 0(s3)
    lw a5 0(s10)
    lw a6 64(sp)
    jal matmul
    
	# Write output matrix o
	lw a0 16(s1)
    lw a1 64(sp)
    lw a2 0(s6)
    lw a3 0(s10)
	jal write_matrix
    
	# Compute and return argmax(o)
	lw a0 64(sp)
    lw a1 60(sp)
    jal argmax
	sw a0 0(sp)
    
	# If enabled, print argmax(o) and newline
    beq s2 x0 print

done:
	lw a0 4(sp)
    jal free
    lw a0 64(sp)
    jal free
	mv a0 s3
    jal free
    mv a0 s5
    jal free
    mv a0 s6
    jal free
    mv a0 s8
    jal free
    mv a0 s9
    jal free
    mv a0 s11
    jal free
    
    lw a0 0(sp)
    
    lw s0 8(sp)
    lw s1 12(sp)
    lw s2 16(sp)
    lw s3 20(sp)
    lw s4 24(sp)
    lw s5 28(sp)
    lw s6 32(sp)
    lw s7 36(sp)
    lw s8 40(sp)
    lw s9 44(sp)
    lw s10 48(sp)
    lw s11 52(sp)
    lw ra 56(sp)
    addi sp sp 68
    ret
    
print:
	lw a0 0(sp)
    jal print_int
    addi t0 x0 '\n'
    mv a0 t0
    jal print_char
    j done
    
arg_err:
	li a0 31
    j exit
malloc_err:
	li a0 26
    j exit

