/*
 * MIT License
 * 
 * Copyright (c) 2019 AVBotz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ========================================================================== */

/** @file matrix.h
 *  @brief Function declarations for matrix operations.
 *
 *  Matrices are stored in 1-D arrays. To access a cell, use [r*n+c] instead 
 *  of [r][c].
 *
 *  @author David Zhang (Davarco)
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>

/** @brief Print real matrix to terminal.
 *
 *  @param A The matrix that is printed.
 *  @param m The number of rows in A.
 *  @param n The number of columns in B.
 *  @param msg The header message preceding the matrix.
 */
void print(float *A, int m, int n);

/** @brief Create an identity matrix of order nxn.
 *
 *  @param A The address where the matrix is written.
 *  @param n The number of rows and columns of A.
 */
void identity(float *A, int n);

/** @brief Copies the elements from one matrix to another.
 *
 *  @param A The matrix from where elements are copied from.
 *  @param m The number of rows in A and B.
 *  @param n The number of columns in A and B.
 *  @param B The matrix where elements are written to.
 */
void copy(float *A, int n, int m, float *B);

/** @brief Multiplies two matrices together.
 *
 *  @param A The first matrix.
 *  @param B The second matrix.
 *  @param m The number of rows in A.
 *  @param p The number of columns in A and number of rows in B.
 *  @param n The number of columns in B.
 *  @param C The multiplied matrix of order mxn.
 */
void multiply(float *A, float *B, int m, int p, int n, float *C);

/** @brief Adds two matrices together.
 *
 *  @param A The first matrix.
 *  @param B The second matrix.
 *  @param m The number of rows in A and B.
 *  @param n The number of columns in A and B.
 *  @param C The added matrix.
 */
void add(float *A, float *B, int m, int n, float *C);

/** @brief Subtracts two matrices.
 *
 *  @param A The first matrix.
 *  @param B The second matrix.
 *  @param m The number of rows in A and B.
 *  @param n The number of columns in A and B.
 *  @param C The subtracted matrix.
 */
void subtract(float *A, float *B, int m, int n, float *C);

/** @brief Takes the transpose of a matrix.
 *
 *  @param A The input matrix to transpose.
 *  @param m The number of rows in A.
 *  @param n The number of columns in A.
 *  @param B The transposed matrix of order nxm.
 */
void transpose(float *A, int m, int n, float *B);

/** @brief Multiplies a matrix by a scalar.
 *
 *  @param A The input matrix, where the results are written to as well.
 *  @param m The number of rows in A.
 *  @param n The number of columns in A.
 *  @param k The scalar value.
 */
void scale(float *A, int m, int n, float k);

/** @brief Takes the inverse of a matrix.
 *
 *  @param A The input matrix, must be square.
 *  @param n The number of rows and columns in A.
 *  @param B The inverted matrix.
 */
int invert(float *A, int n, float *B);

#endif
