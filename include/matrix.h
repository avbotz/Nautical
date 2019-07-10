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
