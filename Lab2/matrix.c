#include "matrix.h"
#include <stdio.h>

#define ERR_ALLOC_MEM "ERROR: don't allocate memory"

Matrix* create_matrix(int rows, int columns) {
	Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
	if (!matrix) {
		printf_s(ERR_ALLOC_MEM);
		_fcloseall();
		exit(8);
	}

	matrix->matrix = (double**)malloc(sizeof(double*) * rows);
	if (!matrix->matrix) {
		printf_s(ERR_ALLOC_MEM);
		_fcloseall();
		exit(1);
	}

	for (int i = 0; i < rows; i++) {
		matrix->matrix[i] = (double*)calloc(sizeof(double), columns);

		if (!matrix->matrix[i]) {
			for (int j = 0; j <= i; j++) free(matrix->matrix[j]);
			free(matrix->matrix);
			free(matrix);
			printf_s(ERR_ALLOC_MEM);
			_fcloseall();
			exit(8);
		}
	}

	matrix->rows = rows;
	matrix->columns = columns;

	return matrix;
}
void delete_matrix(Matrix* matrix) {
        for (int i = 0; i < matrix->rows; i++) free(matrix->matrix[i]);
        free(matrix->matrix);
        free(matrix);
}

Matrix* cofactor(Matrix* matrix, int delete_row, int delete_column) {
	int rows = matrix->rows;
	int columns = matrix->columns;

	Matrix* cofactor_matrix = create_matrix(rows - 1, columns - 1);

	int row = 0;
	int column = 0;
	for (int i = 0; i < rows; i++) {
		if (i == delete_row) continue;
		for (int j = 0; j < columns; j++) {
			if (j == delete_column) continue;
			cofactor_matrix->matrix[row][column++] = matrix->matrix[i][j];
		}
		row++;
		column = 0;
	}

	return cofactor_matrix;
}

Matrix* matrix_addition(Matrix* first_matrix, Matrix* second_matrix) {
	for (int i = 0; i < first_matrix->rows; i++) {
		for (int j = 0; j < first_matrix->columns; j++) {
			first_matrix->matrix[i][j] += second_matrix->matrix[i][j];
		}
	}

	return first_matrix;
}
Matrix* matrix_substraction(Matrix* first_matrix, Matrix* second_matrix) {
	for (int i = 0; i < first_matrix->rows; i++) {
		for (int j = 0; j < first_matrix->columns; j++) {
			first_matrix->matrix[i][j] -= second_matrix->matrix[i][j];
		}
	}

	return first_matrix;
}
Matrix* matrix_multiplication(Matrix* first_matrix, Matrix* second_matrix) {
	int rows = first_matrix->rows;
	int columns = second_matrix->columns;
	Matrix* result_matrix = create_matrix(rows, columns);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			for (int k = 0; k < first_matrix->columns; k++) {
				result_matrix->matrix[i][j] += first_matrix->matrix[i][k] * second_matrix->matrix[k][j];
			}
		}
	}

	return result_matrix;
}
Matrix* matrix_transposition(Matrix* matrix) {
	int columns = matrix->rows;
	int rows = matrix->columns;
	Matrix* result_matrix = create_matrix(rows, columns);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			result_matrix->matrix[i][j] = matrix->matrix[j][i];
		}
	}

	return result_matrix;
}
double matrix_determinant(Matrix* matrix) {
	int order = matrix->columns;
	if (order == 1) return matrix->matrix[0][0];
	else if (order == 2) return (matrix->matrix[0][0] * matrix->matrix[1][1]) - (matrix->matrix[0][1] * matrix->matrix[1][0]);
	else {
		double det = 0;
		int sign = 1;
		for (int i = 0; i < order; i++) {
			Matrix* cofactor_matrix = cofactor(matrix, 0, i);
			det += sign * matrix->matrix[0][i] * matrix_determinant(cofactor_matrix);
                        delete_matrix(cofactor_matrix);
			sign *= -1;
		}

		return det;
	}
}
