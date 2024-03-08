#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col) {
   double* matrix_elts = mat->data;
   return matrix_elts[col + row * mat->cols]; 
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
   double* matrix_elts = mat->data;
   matrix_elts[col + row * mat->cols] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    // Task 1.2 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Allocate space for the matrix data, initializing all entries to be 0. Return -2 if allocating memory failed.
    // 4. Set the number of rows and columns in the matrix struct according to the arguments provided.
    // 5. Set the `parent` field to NULL, since this matrix was not created from a slice.
    // 6. Set the `ref_cnt` field to 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0) {
	    return -1;
    }
    matrix* new_matrix = (matrix*) malloc(sizeof(matrix));
    if (new_matrix == NULL) {
	    return -2;
    }
    double* matrix_data = (double*) calloc(cols*rows, sizeof(double));
    if (matrix_data == NULL) {
	    return -2;
    }
    new_matrix->data = matrix_data;
    new_matrix->cols = cols;
    new_matrix->rows = rows;
    new_matrix->parent = NULL;
    new_matrix->ref_cnt = 1;
    *mat = new_matrix;
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
    if (mat == NULL) {
	    return;
    }
    if (mat->parent == NULL) {
	    mat->ref_cnt -= 1;
	    if (mat->ref_cnt == 0) {
		    free (mat->data);
		    free(mat);
	    }
    }
    else {
    	deallocate_matrix(mat->parent);
    	free(mat);
    }
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0) {
	    return -1;
    }
    matrix* new_matrix = (matrix*) malloc(sizeof(matrix));
    if (new_matrix == NULL) {
	    return -2;
    }
    double* from_matrix_elts = from->data;
    new_matrix->data = from_matrix_elts + offset;
    new_matrix->cols = cols;
    new_matrix->rows = rows;
    new_matrix->parent = from;
    new_matrix->ref_cnt = 1;
    from->ref_cnt += 1;
    *mat = new_matrix;
    return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
    __m256d filler = _mm256_set1_pd(val);
    int tot_elts = mat->rows * mat->cols;
    int elts = tot_elts / 4;
    // we set all the values by parallelizing the index and incrementing elt by elt
    #pragma omp parallel for 
    for (int i  = 0; i < elts * 4; i += 4) {
	_mm256_storeu_pd(mat->data + i, filler);
    }
    for (int i = elts * 4; i < tot_elts; i++) {
	    mat->data[i] = val;
    }
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {/*
   int tot_elts = mat->cols * mat->rows;
   int elts = tot_elts / 4;
   #pragma omp parallel for
   for (int i = 0; i < elts * 4; i += 4) {
	double abs_vals[4]; // declare array to hold the vals
	double first = mat->data[i];
	double second = mat->data[i+1];
	double third = mat->data[i+2];
	double fourth = mat->data[i+3]; // get all the matrix vales and store them
	__m256d initial = _mm256_set_pd(fourth, third, second, first);
	__m256d negatedinit = _mm256_set_pd(-fourth, -third, -second, -first);
	_mm256_storeu_pd(abs_vals, _mm256_max_pd(initial, negatedinit)); // take the max to only get positive nums
	result->data[i] = abs_vals[0];
	result->data[i+1] = abs_vals[1];
	result->data[i+2] = abs_vals[2];
	result->data[i+3] = abs_vals[3]; // store all these numbers in our result
   }
   // if not a multiple of 4 then the last 3 or less remaining elts will go through this regular for loop
   for (int i = elts * 4; i < tot_elts; i++) {
	   if (mat->data[i] < 0) {
		   result->data[i] = mat->data[i] * -1;
	   }
	   else {
		   result->data[i] = mat->data[i];
	   }
   }
   return 0; */
//we are now taking a new approach in order to take advantage of the cache and minimize the time it takes for the function to run through
    int tot_elts = mat->rows * mat->cols;
    int elts = tot_elts / 4;
    __m256d inverse =_mm256_set1_pd(-1.0); //we need a negative 1 float in order to get the negative of our element
    #pragma omp parallel for //WE NEED TO USE data + i to access in loadu because it loads from ARRAY 
    for (int i = 0; i < elts * 4; i += 4) {
	__m256d elt = _mm256_loadu_pd(mat->data + i); //store element in matrix pos i
	__m256d neg_elt = _mm256_mul_pd(elt, inverse); // store negative element in matrix pos i
	__m256d pos_elt = _mm256_max_pd(elt, neg_elt); // get the positive version of the element by maxing the 2
	_mm256_storeu_pd(result->data + i, pos_elt); // store this back in our matrix of result
    }
    for (int i = elts * 4; i < tot_elts; i++) { // loop to go through the last less than 4 elts of our matrix
	if (mat->data[i] < 0) {
	    result->data[i] = -1 * mat->data[i];
	} else {
	    result->data[i] = mat->data[i];
	}
    }
    return 0;

}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /*int tot_elts = mat2->cols * mat2->rows;
    int elts = tot_elts / 4;
    #pragma omp parallel for 
    for (int i  = 0; i < elts * 4; i += 4) {
	double add_elts[4]; // array for holding our added matrix values
	double m1e1 = mat1->data[i];
	double m1e2 = mat1->data[i+1];
	double m1e3 = mat1->data[i+2];
	double m1e4 = mat1->data[i+3];
	double m2e1 = mat2->data[i];
	double m2e2 = mat2->data[i+1];
	double m2e3 = mat2->data[i+2];
	double m2e4 = mat2->data[i+3]; // store first 4 values of both matricies according to current index i
	__m256d add = _mm256_add_pd(_mm256_set_pd(m1e4, m1e3, m1e2, m1e1), _mm256_set_pd(m2e4, m2e3, m2e2, m2e1));
	_mm256_storeu_pd(add_elts, add); // add these values and store in our array
	result->data[i] = add_elts[0];
	result->data[i+1] = add_elts[1];
	result->data[i+2] = add_elts[2];
	result->data[i+3] = add_elts[3]; //load values into result array
    }
    // any elts not in mul of 4, so last 3 or less go through this for loop
    for (int i = elts; i < tot_elts; i++) {
	    result->data[i] = mat1->data[i] + mat2->data[i];
    }
    return 0;*/
//we now take anew approach of not loop unrolling the exact same way but in order to take advantage of the cache as best as possible
    int tot_elts = mat1->rows * mat1->cols;
    int elts = tot_elts / 4;
    #pragma omp parallel for //WE NEED TO USE data + i to access in loadu because it loads from ARRAY
    for (int i = 0; i < elts * 4; i+=4) {
	__m256d m1elt = _mm256_loadu_pd(mat1->data + i); // get element in matrix 1
	__m256d m2elt = _mm256_loadu_pd(mat2->data + i); // get elt in matrix 2
	__m256d added = _mm256_add_pd(m1elt, m2elt); // add the elts
	_mm256_storeu_pd(result->data + i, added); // store this in result
    }
    for (int i = elts * 4; i < tot_elts; i++) {
	result->data[i] = mat1->data[i] + mat2->data[i];
    }
    return 0;

}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {/* 
  //as with lab we want to maximize the number of cache hits and to do so we transpose the matrix
  //create new matrix and allocate space and set it up to be transposed version of mat2
  matrix* transposed;
  allocate_matrix(&transposed, mat2->cols, mat2->rows);
  int rows = mat2->rows;
  int cols = mat2->cols;
  //#pragma omp parallel for 
  for (int i = 0; i < rows; i++) {
	  for (int j = 0; j < cols; j++) {
		 transposed->data[i + j*rows] = mat2->data[i*cols + j];
	  }
  }
  int imax = mat1->rows;
  fill_matrix(result, 0);
  #pragma omp parallel for
  for (int i = 0; i < imax; i++) { // we are iterating through all the rows
	  //#pragma omp parallel for
	  for (int j = 0; j < transposed->rows; j++) { // we are iterating through all the columns
		  int tot_elts = mat1->cols;
		  int elts = tot_elts / 4;
		  int tcols = transposed->cols;
		  int jmax = transposed->rows;
		  double result_elt = 0; // we are holding the value to assign after all multiplication of row col is done
		  //#pragma omp parallel for
		  for (int chk = 0; chk < elts * 4; chk += 4) { // we are iterating through chunks of 4 until reaching the end of a column
			double mul_vals[4]; // matrix to store the multiplied sets of 4 matrix elts
			double m1e1 = mat1->data[i*tot_elts + chk];
			double m1e2 = mat1->data[i*tot_elts + chk + 1];
		       	double m1e3 = mat1->data[i*tot_elts + chk + 2];
	       		double m1e4 = mat1->data[i*tot_elts + chk + 3];
		 	double m2e1 = transposed->data[j*tcols + chk];
			double m2e2 = transposed->data[j*tcols + chk + 1];
			double m2e3 = transposed->data[j*tcols + chk + 2];
			double m2e4 = transposed->data[j*tcols + chk + 3];
			__m256d mul = _mm256_mul_pd(_mm256_set_pd(m1e4, m1e3, m1e2, m1e1) , _mm256_set_pd(m2e4, m2e3, m2e2, m2e1));
			_mm256_storeu_pd(mul_vals, mul); // we are multiplying the elts and storing them
			for (int i = 0; i < 4; i++) {	
				result_elt += mul_vals[i];
			}
		  } // adding all the elts to the result then continue
		  for (int chk = elts * 4; chk < tot_elts; chk++) {
			result_elt += mat1->data[i*tot_elts + chk] * transposed->data[j*tcols + chk];
		  } //for the remaining elts of the col we will use this for loop then finally add everything to the result matrix
		  result->data[i*jmax + j] = result_elt;
	  }
  }
  deallocate_matrix(transposed); //deallocate any mallocd data from the transposed matrix
  return 0; */
// we are taking a simpler route of filling accoridng to what is currently in the cache and adding onto 0 to avoid any invalid entries in the resutl matrix HERE:::
    fill_matrix(result, 0);
    size_t i;
    size_t j;
    size_t k;
    #pragma omp parallel for
    for (i = 0; i < mat1->rows; i++) {
        for (j = 0; j < mat1->cols; j++) {
            for (k = 0; k < mat2->cols; k++) {
                result->data[i*result->cols + k] += mat1->data[i*mat1->cols + j] * mat2->data[j*mat2->cols + k];
            }
        }
    }
    return 0;
     
}


/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    matrix* mat_mul;
    allocate_matrix(&mat_mul, mat->rows, mat->cols);
    matrix* mat_mul_holder;
    allocate_matrix(&mat_mul_holder, mat->rows, mat->cols);
//we declare 2 new matricies, one to hold our current matmul and one to hold our actual matrix multiplier
    int rows = mat->rows;
    int cols = mat->cols;
    double* result_data = result->data;
    #pragma omp parallel for
    for (size_t i = 0; i < rows; i++) {
	for (size_t j = 0; j < cols; j++) {
		mat_mul->data[i * mat->cols + j] = mat->data[i * mat->cols + j];
		if (i == j) {
		    result_data[i * mat->cols + j] = 1;
		} 
		else {
		    result_data[i * mat->cols + j] = 0;
		}
	}
    } // basically setting result and the identity matrix both to 0 and mat_mul = mat
    while (pow > 0) {
	    if (pow % 2 == 1) {
		    mul_matrix(mat_mul_holder, mat_mul, result);
		    //#pragma omp parallel for
    		    for (size_t i = 0; i < mat->rows; i++) {
        	    	for (size_t j = 0; j < mat->cols; j++) {
            	    		result->data[i * mat->cols + j] = mat_mul_holder->data[i * mat->cols + j];
			}
		    }
		    pow -= 1;
	    }
	    else {
		    mul_matrix(mat_mul_holder, mat_mul, mat_mul);
		    //#pragma omp parallel for
    		    for (size_t i = 0; i < mat->rows; i++) {
        		for (size_t j = 0; j < mat->cols; j++) {
            			mat_mul->data[i * mat->cols + j] = mat_mul_holder->data[i * mat->cols + j];
			}
		    }	
		    pow /= 2;
	    }
    } // this will go through and do mat^2 for multiples of 2 and multply mat mul by mat mul to keep the powers going and will just do mat mul times result at the very end and or the very start in order to save to result
    deallocate_matrix(mat_mul);
    deallocate_matrix(mat_mul_holder); //deallocate all allocated matricies
    return 0;
}
