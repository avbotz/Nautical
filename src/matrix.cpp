
/*
 *  MatrixMath.cpp Library for Matrix Math
 *
 *  Modified by David Zhang to fit Nautical.
 *  Created by Charlie Matlack on 12/18/10.
 *  Modified from code by RobH45345 on Arduino Forums, algorithm from
 *  NUMERICAL RECIPES: The Art of Scientific Computing.
 */
#include "matrix.h"
#define NR_END 1


void print(float* A, int m, int n)
{
	int i, j;
	Serial.println();
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			Serial.print(A[n * i + j]);
			Serial.print("\t");
		}
		Serial.println();
	}
}

void copy(float* A, int n, int m, float* B)
{
	int i, j, k;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
		{
			B[n * i + j] = A[n * i + j];
		}
}

void multiply(float* A, float* B, int m, int p, int n, float* C)
{
	int i, j, k;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
		{
			C[n * i + j] = 0;
			for (k = 0; k < p; k++)
				C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
		}
}

void add(float* A, float* B, int m, int n, float* C)
{
	int i, j;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			C[n * i + j] = A[n * i + j] + B[n * i + j];
}

void subtract(float* A, float* B, int m, int n, float* C)
{
	int i, j;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			C[n * i + j] = A[n * i + j] - B[n * i + j];
}

void transpose(float* A, int m, int n, float* C)
{
	int i, j;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			C[m * j + i] = A[n * i + j];
}

void scale(float* A, int m, int n, float k)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			A[n * i + j] = A[n * i + j] * k;
}

int invert(float* A, int n)
{
	int pivrow;		
	int k, i, j;			
	int pivrows[n]; 	
	float tmp;		
	for (k = 0; k < n; k++)
	{
		tmp = 0;
		for (i = k; i < n; i++)
		{
			if (abs(A[i * n + k]) >= tmp)
			{
				tmp = abs(A[i * n + k]);
				pivrow = i;
			}
		}

		if (A[pivrow * n + k] == 0.0f)
		{
			Serial.println("Inversion failed due to singular matrix");
			return 0;
		}

		if (pivrow != k)
		{
			for (j = 0; j < n; j++)
			{
				tmp = A[k * n + j];
				A[k * n + j] = A[pivrow * n + j];
				A[pivrow * n + j] = tmp;
			}
		}
		pivrows[k] = pivrow;	
		tmp = 1.0f / A[k * n + k];
		A[k * n + k] = 1.0f;

		for (j = 0; j < n; j++)
			A[k * n + j] = A[k * n + j] * tmp;

		for (i = 0; i < n; i++)
		{
			if (i != k)
			{
				tmp = A[i * n + k];
				A[i * n + k] = 0.0f; 
				for (j = 0; j < n; j++)
				{
					A[i * n + j] = A[i * n + j] - A[k * n + j] * tmp;
				}
			}
		}
	}

	for (k = n - 1; k >= 0; k--)
	{
		if (pivrows[k] != k)
		{
			for (i = 0; i < n; i++)
			{
				tmp = A[i * n + k];
				A[i * n + k] = A[i * n + pivrows[k]];
				A[i * n + pivrows[k]] = tmp;
			}
		}
	}
	return 1;
}
