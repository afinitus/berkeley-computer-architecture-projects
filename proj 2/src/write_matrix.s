.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

	addi sp sp -28
    sw s1 0(sp)
    sw s2 4(sp)
    sw s3 8(sp)
    sw s4 12(sp)
    sw s5 16(sp)
    sw s6 20(sp)
    sw ra 24(sp)

	mv s1 a1
    mv s2 a2
    mv s3 a3
    
    #open file in write mode and save it
	li a1 1
    jal fopen
    li t0 -1
    beq a0 t0 open_err
    mv s4 a0
    
    #malloc space for the row column inputs
    addi a0 x0 8
    jal malloc 
    mv s5 a0
    
    #write the row col stuff in
    sw s2 0(s5)
    sw s3 4(s5)
    mv a0 s4
    mv a1 s5
    li a2 2
    li a3 4
    jal fwrite
    li a2 2
    bne a0 a2 write_err
    
    #write the rest of the matrix stuff in
    mul s6 s2 s3
    mv a0 s4
    mv a1 s1
    mv a2 s6
    li a3 4
    jal fwrite
    bne a0 s6 write_err
    
    #close it all up
    mv a0 s4
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
write_err:
	li a0 30
    j exit

done:
	mv s6 a0
    mv a0 s5
    jal free
    mv a0 s6
    lw s1 0(sp)
    lw s2 4(sp)
    lw s3 8(sp)
    lw s4 12(sp)
    lw s5 16(sp)
    lw s6 20(sp)
    lw ra 24(sp)
    addi sp sp 28
	ret
