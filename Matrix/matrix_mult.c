#include "s21_matrix.h"

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = 0;
  if (!A || !A->matrix || !result) return ERR_MAT;
  if (!isfinite(number)) return ERR_CALC;
  if (s21_create_matrix(A->rows, A->columns, result) != 0) return ERR_MAT;

  for (int i = 0; i < A->rows; i++) {
    for (int k = 0; k < A->columns; k++) {
      if (!isfinite(A->matrix[i][k])) {
        s21_remove_matrix(result);
        status = ERR_CALC;
      } else {
        double value = A->matrix[i][k] * number;
        result->matrix[i][k] = ROUND_6(value);
      }
    }
  }
  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;
  if (!A || !A->matrix || !B || !B->matrix || !result) return ERR_MAT;
  if (A->columns != B->rows) return ERR_CALC;
  if (s21_create_matrix(A->rows, B->columns, result) != 0) return ERR_MAT;

  for (int i = 0; i < A->rows && status == 0; i++) {
    for (int j = 0; j < B->columns && status == 0; j++) {
      double sum = 0.0;
      for (int k = 0; k < A->columns && status == 0; k++) {
        if (!isfinite(A->matrix[i][k]) || !isfinite(B->matrix[k][j])) {
          s21_remove_matrix(result);
          status = ERR_CALC;
          continue;
        }
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      if (status == 0) {
        result->matrix[i][j] = ROUND_6(sum);
      }
    }
  }
  return status;
}
