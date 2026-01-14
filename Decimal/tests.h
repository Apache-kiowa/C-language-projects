#ifndef SRC_S21_TEST_H_
#define SRC_S21_TEST_H_

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define S21_ERROR 1
#include "s21_decimal.h"

#define SUITE_NAME "Unit"
#define TCASE_NAME "Tests"

TCase *tests_s21_int_to_decimal(void);
TCase *tests_s21_decimal_to_int(void);
TCase *tests_s21_float_to_decimal(void);
TCase *tests_s21_decimal_to_float(void);
TCase *tests_s21_negate(void);
TCase *tests_s21_mul(void);
TCase *tests_s21_add(void);
TCase *tests_s21_sub(void);
TCase *tests_s21_div(void);
TCase *tests_s21_truncate(void);
TCase *tests_s21_floor(void);
TCase *tests_s21_round(void);
TCase *tests_s21_comparison(void);

#endif
