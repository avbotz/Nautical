/*
 *  MatrixMath.h Library for Matrix Math
 *
 *  Modified by David Zhang to fit Nautical.
 *  Created by Charlie Matlack on 12/18/10.
 *  Modified from code by RobH45345 on Arduino Forums, algorithm from
 *  NUMERICAL RECIPES: The Art of Scientific Computing.
 *  Modified to work with Arduino 1.0/1.5 by randomvibe & robtillaart
 *  Made into a real library on GitHub by Vasilis Georgitzikis (tzikis)
 *  so that it's easy to use and install (March 2015)
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>

void print(float* A, int m, int n);
void copy(float* A, int n, int m, float* B);
void multiply(float* A, float* B, int m, int p, int n, float* C);
void add(float* A, float* B, int m, int n, float* C);
void subtract(float* A, float* B, int m, int n, float* C);
void transpose(float* A, int m, int n, float* C);
void scale(float* A, int m, int n, float k);
int invert(float* A, int n);

#endif
