#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = 0;
  if (!result || rows <= 0 || columns <= 0) return ERR_MAT;
  result->rows = rows;
  result->columns = columns;
  result->matrix = malloc(rows * sizeof(double *));
  if (!result->matrix) {
    status = ERR_MAT;
  } else {
    for (int i = 0; i < rows; i++) result->matrix[i] = NULL;
    status = filling_matrix(result);
  }

  return status;
}
int filling_matrix(matrix_t *result) {
  int status = 0;
  for (int i = 0; i < result->rows && !status; i++) {
    result->matrix[i] = malloc(result->columns * sizeof(double));
    if (!result->matrix[i]) {
      status = ERR_MAT;
      s21_remove_matrix(result);
    } else {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = 0.0;
      }
    }
  }
  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (!A || !A->matrix) return;
  for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
  free(A->matrix);
  *A = (matrix_t){0};
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;
  if ((!A || !A->matrix) || (!B || !B->matrix)) return FAILURE;
  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)
    return FAILURE;
  if ((A->rows != B->rows) || (A->columns != B->columns)) return FAILURE;
  for (int i = 0; i < A->rows; i++) {
    for (int k = 0; k < A->columns; k++) {
      if (fabs(A->matrix[i][k] - B->matrix[i][k]) > EPS) status = FAILURE;
    }
  }
  return status;
}

void s21_initialize_matrix(matrix_t *A, double start_value, double step) {
  if (A == NULL && A->matrix == NULL) return;
  double value = start_value;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = value;
      value += step;
    }
  }
}