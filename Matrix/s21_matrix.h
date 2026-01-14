#ifndef S21_MATRIX_H
#define S21_MATRIX_H

// LIB
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// MAKROS
#define EPS 1e-6
#define ROUND_6(x) (round((x) * 1e6) / 1e6)
#define OK 0
#define ERR_MAT 1
#define ERR_CALC 2

#define SUCCESS 1
#define FAILURE 0
// STRUCT
typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// FUNCTION
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
// UTILS
int filling_matrix(matrix_t *result);
void get_minor(matrix_t *A, int row, int col, matrix_t *minor);
double det_recursive(matrix_t *A);
int subOurSum(matrix_t *A, matrix_t *B, matrix_t *result, int sum_flag);
void s21_initialize_matrix(matrix_t *A, double start_value, double step);
#endif