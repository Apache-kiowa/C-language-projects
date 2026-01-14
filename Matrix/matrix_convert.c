#include "s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = OK;
  if (!A || !A->matrix || !result || A->rows <= 0 || A->columns <= 0)
    return ERR_MAT;
  if (s21_create_matrix(A->columns, A->rows, result) != 0) {
    status = ERR_MAT;
  }
  for (int i = 0; i < A->rows && status != ERR_MAT; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }

  return status;
}

void get_minor(matrix_t *A, int row, int col, matrix_t *minor) {
  s21_create_matrix(A->rows - 1, A->columns - 1, minor);
  int r = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) continue;

    int c = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) continue;

      minor->matrix[r][c] = A->matrix[i][j];
      c++;
    }
    r++;
  }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (!A || !result) return ERR_MAT;
  s21_create_matrix(A->rows, A->columns, result);
  if (A->rows != A->columns) return ERR_CALC;

  if (A->rows == 1) {
    result->matrix[0][0] = 1;
    return 0;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t minor;
      get_minor(A, i, j, &minor);

      double det = 0;
      s21_determinant(&minor, &det);

      result->matrix[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * det;
      s21_remove_matrix(&minor);
    }
  }

  return OK;
}

double det_recursive(matrix_t *A) {
  if (A->rows == 1) return A->matrix[0][0];
  if (A->rows == 2)
    return A->matrix[0][0] * A->matrix[1][1] -
           A->matrix[0][1] * A->matrix[1][0];

  double det = 0;
  for (int j = 0; j < A->columns; j++) {
    matrix_t minor;
    get_minor(A, 0, j, &minor);
    double cofactor = ((j % 2 == 0) ? 1 : -1) * det_recursive(&minor);
    det += A->matrix[0][j] * cofactor;

    for (int i = 0; i < minor.rows; i++) free(minor.matrix[i]);
    free(minor.matrix);
  }
  return det;
}

int s21_determinant(matrix_t *A, double *result) {
  if (!A || !result || !A->matrix || A->rows <= 0 || A->columns <= 0)
    return ERR_MAT;
  if (A->rows != A->columns) return ERR_CALC;

  *result = det_recursive(A);
  return 0;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (!A || !A->matrix || !result) return ERR_MAT;
  if (A->rows != A->columns) return ERR_CALC;

  double det = 0;
  if (s21_determinant(A, &det) != OK) return ERR_CALC;
  if (fabs(det) < EPS) return ERR_CALC;

  matrix_t complements = {0};
  matrix_t transposed = {0};

  if (s21_calc_complements(A, &complements) != OK) return ERR_CALC;
  if (s21_transpose(&complements, &transposed) != OK) {
    s21_remove_matrix(&complements);
    return ERR_CALC;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK) {
    s21_remove_matrix(&complements);
    s21_remove_matrix(&transposed);
    return ERR_CALC;
  }

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->matrix[i][j] = transposed.matrix[i][j] / det;
    }
  }

  s21_remove_matrix(&complements);
  s21_remove_matrix(&transposed);
  return OK;
}
