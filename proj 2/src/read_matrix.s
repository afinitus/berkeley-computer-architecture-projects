.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:
	addi sp sp -24
    sw s1 0(sp)
    sw s2 4(sp)
    sw s3 8(sp)
    sw s4 12(sp)
    sw s5 16(sp)
    sw ra 20(sp)
	
    mv s1 a1
    mv s2 a2
    
    #open the file in read mode
    li a1 0
    jal fopen
    li t0 -1
    beq a0 t0 open_err
    
    #read the contents of the file to get row col amount
	mv s3 a0
    mv a1 s1
	li a2 4
    jal fread
	li a2 4
    bne a0 a2 read_err
    mv a1 s2
    mv a0 s3
    jal fread
    li a2 4
    bne a0 a2 read_err
    
    #malloc space for row * col * sizeof(int) and read that
    lw t0 0(s1)
    lw t1 0(s2)
    mul t0 t0 t1
    li t1 4
    mul s4 t0 t1
    mv a0 s4
    jal malloc
    beq a0 x0 malloc_err
    mv s5 a0
    mv a0 s3
    mv a1 s5
    mv a2 s4
    jal fread
    bne a0 s4 read_err
    
    #close it all up
    mv a0 s3
    jal fclose
    li t0 -1
    beq a0 t0 close_err
    j done

malloc_err:
	li a0 26
    j exit
open_err:
    li a0 27
    j exit
close_err:
    li a0 28
    j exit
read_err:
    li a0 29
    j exit
    
done:
	mv a0 s5
    lw s1 0(sp)
    lw s2 4(sp)
    lw s3 8(sp)
    lw s4 12(sp)
    lw s5 16(sp)
    lw ra 20(sp)
    addi sp sp 24
	ret
