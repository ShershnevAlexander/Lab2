#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN_FILENAME 256
#define MAX_LEN_ROW 256

#define ERR_OPEN_FILE "ERROR: could not open the file"
#define ERR_EMPTY_FILE "ERROR: file is empty"
#define ERR_MATRIX "ERROR: this is not a matrix"
#define ERR_ADD_MATRIX "ERROR: matrices cannot be added/substracted"
#define ERR_MULT_MATRIX "ERROR: matrices cannot be multiplied"
#define ERR_DET_MATRIX "ERROR: determinant cannot be calculated"

FILE* open_file(char* filename);
int rows_count(FILE* file);
int column_count(FILE* file, int rows);
Matrix* get_matrix();
int check_matrix_for_det(Matrix* matrix);
int check_matrices_for_mult(Matrix* first_matrix, Matrix* second_matrix);
int check_matrices_for_add(Matrix* first_matrix, Matrix* second_matrix);
void print_matrix(Matrix* matrix);

int main() {
	int action = 0;
	printf_s("Matrix calculator\n\n"
			  "Choose an action:\n"
			  "1. Matrix addition\n"
			  "2. Matrix substraction\n"
			  "3. Matrix multiplication\n"
			  "4. Transpose the matrix\n"
			  "5. Find the determinant of the matrix\n\n");
	scanf_s("%d", &action);
	
	switch (action)
	{
	case 1:
		printf_s("Matrix addition:\n");
		Matrix* first_matrix_add = get_matrix();
		Matrix* second_matrix_add = get_matrix();

		if (check_matrices_for_add(first_matrix_add, second_matrix_add)) {
			Matrix* result_matrix_add = matrix_addition(first_matrix_add, second_matrix_add);
			print_matrix(result_matrix_add);
		}
		else {
			printf_s(ERR_ADD_MATRIX);
			exit(1);
		}
		
		break;
	case 2:
		printf_s("Matrix substraction:\n");
		Matrix* first_matrix_sub = get_matrix();
		Matrix* second_matrix_sub = get_matrix();

		if (check_matrices_for_add(first_matrix_sub, second_matrix_sub)) {
			Matrix* result_matrix_sub = matrix_substraction(first_matrix_sub, second_matrix_sub);
			print_matrix(result_matrix_sub);
		}
		else {
			printf_s(ERR_ADD_MATRIX);
			exit(2);
		}

		break;
	case 3:
		printf_s("Matrix multiplication:\n");
		Matrix* first_matrix_mult = get_matrix();
		Matrix* second_matrix_mult = get_matrix();

		if (check_matrices_for_mult(first_matrix_mult, second_matrix_mult)) {
			Matrix* result_matrix_mult = matrix_multiplication(first_matrix_mult, second_matrix_mult);
			print_matrix(result_matrix_mult);
		}
		else {
			printf_s(ERR_MULT_MATRIX);
			exit(3);
		}

		break;
	case 4:
		printf_s("Transpose the matrix:\n");
		Matrix* matrix_t = get_matrix();
		Matrix* result_matrix_t = matrix_transposition(matrix_t);
		print_matrix(result_matrix_t);

		break;
	case 5:
		printf_s("Find the determinant of the matrix:\n");
		Matrix* matrix_det = get_matrix();
		if (check_matrix_for_det(matrix_det)) {
			double det = matrix_determinant(matrix_det);
			printf_s("%lf", det);
		}
		else {
			printf_s(ERR_DET_MATRIX);
			exit(4);
		}

		break;
	default:
		printf_s("Incorrect input\n");
		break;
	}

	return 0;
}

FILE* open_file(char* filename) {
	FILE* file;
	if (fopen_s(&file, filename, "r")) {
		printf_s(ERR_OPEN_FILE);
		_fcloseall();
		exit(5);
	}

	if (fgetc(file) == EOF) {
		printf_s(ERR_EMPTY_FILE);
		_fcloseall();
		exit(6);
	}

	fseek(file, 0, SEEK_SET);
	return file;
}
int rows_count(FILE* file) {
	char str[MAX_LEN_ROW];
	int count = 0;
	while (fgets(str, MAX_LEN_ROW, file)) {
		if (str[0] != '\n') count++;
	}

	fseek(file, 0, SEEK_SET);
	return count;
}
int column_count(FILE* file, int rows) {
	char str[MAX_LEN_ROW];
	int main_columns = 1;
	for (int i = 0; i < rows; i++) {
		int columns = 1;
		int space_count = 0;

		fgets(str, MAX_LEN_ROW, file);
		if (str[0] == '\n') continue;
		int len = strlen(str);

		for (int j = 0; j < len; j++) {
			if (isspace(str[j]) && space_count) continue;
			if (isspace(str[j])) space_count++;
			else {
				columns += space_count;
				space_count = 0;
			}
		}
		if (i == 0) main_columns = columns;
		if (main_columns != columns) {
			printf_s(ERR_MATRIX);
			exit(7);
		}
	}
	fseek(file, 0, SEEK_SET);
	return main_columns;
}
Matrix* get_matrix() {
	char filename[MAX_LEN_FILENAME];
	FILE* file;
	printf_s("Write name of file with matrix: ");
	scanf_s("%s", &filename, MAX_LEN_FILENAME);
	
	file = open_file(filename);

	int rows = rows_count(file);
	int columns = column_count(file, rows);

	Matrix* matrix = create_matrix(rows, columns);
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < rows; j++) {
			fscanf_s(file, "%lf", &matrix->matrix[i][j]);
		}
	}

	fclose(file);
	return matrix;
}
int check_matrix_for_det(Matrix* matrix) {
	return matrix->columns == matrix->rows;
}
int check_matrices_for_mult(Matrix* first_matrix, Matrix* second_matrix) {
	return first_matrix->columns == second_matrix->rows;
}
int check_matrices_for_add(Matrix* first_matrix, Matrix* second_matrix) {
	return ((first_matrix->columns == second_matrix->columns) && (first_matrix->rows == second_matrix->rows));
}
void print_matrix(Matrix* matrix) {
	for (int i = 0; i < matrix->rows; i++) {
		for (int j = 0; j < matrix->columns; j++) {
			printf_s("%.2lf\t", matrix->matrix[i][j]);
		}
		printf_s("\n");
	}
}