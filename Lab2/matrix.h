#pragma once

#include <stdlib.h>

struct Matrix {
	int columns;
	int rows;
	double** matrix;
};

typedef struct Matrix Matrix;

Matrix* create_matrix(int rows, int columns);
void delete_matrix(Matrix* matrix);

Matrix* cofactor(Matrix* matrix, int delete_row, int delete_column);

Matrix* matrix_addition(Matrix* first_matrix, Matrix* second_matrix);
Matrix* matrix_substraction(Matrix* first_matrix, Matrix* second_matrix);
Matrix* matrix_multiplication(Matrix* first_matrix, Matrix* second_matrix);
Matrix* matrix_transposition(Matrix* matrix);
double matrix_determinant(Matrix* matrix);
