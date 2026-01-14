#include "s21_matrix.h"

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return subOurSum(A, B, result, 1);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return subOurSum(A, B, result, 0);
}
int subOurSum(matrix_t *A, matrix_t *B, matrix_t *result, int sum_flag) {
  int status = OK;

  if (!A || !A->matrix || !B || !B->matrix || !result) return ERR_MAT;
  if (A->rows != B->rows || A->columns != B->columns) return ERR_CALC;

  if (s21_create_matrix(A->rows, A->columns, result) != 0) return ERR_MAT;
  for (int i = 0; i < A->rows && status == 0; i++) {
    for (int k = 0; k < A->columns && status == 0; k++) {
      if (!isfinite(A->matrix[i][k]) || !isfinite(B->matrix[i][k])) {
        s21_remove_matrix(result);
        status = ERR_CALC;
        continue;
      }
      result->matrix[i][k] = sum_flag ? A->matrix[i][k] + B->matrix[i][k]
                                      : A->matrix[i][k] - B->matrix[i][k];
    }
  }
  return status;
}