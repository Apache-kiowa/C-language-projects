#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
  int scale;
  int is_negative;
  int leading_zeros;
  int highest_bit_position;
  int lowest_bit_position;
} s21_bigDecimal;

#define S21_ERROR 1
#define S21_OK 0
#define S21_MINUS 0b10000000000000000000000000000000

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void clear_decimal(s21_decimal *value);
int get_scale_from_decimal(s21_decimal value);
int get_bit_at_position(s21_decimal value, unsigned char bit);
int get_sign_bit(s21_decimal num);
void set_bit_at_position(s21_decimal *num, int bit, int value);
void set_sign_bit(s21_decimal *num, int sign);
void toggle_sign(s21_decimal *value);
void set_scale_to_decimal(s21_decimal *dec, int scale);
int is_decimal_zero(s21_decimal value);
void print_decimal_info(s21_decimal *dec);
void print_decimal_bits(const s21_decimal *dec);
int get_bit_at_positionBig(s21_bigDecimal *value, unsigned char bit);

void convert_decimal_to_big(s21_decimal decimal, s21_bigDecimal *big_decimal);
int convert_big_to_decimal(s21_bigDecimal *big_decimal, s21_decimal *decimal);

void clear_big_decimal(s21_bigDecimal *value);
void clear_big_mantissa(s21_bigDecimal *value);
int get_big_bit_at_position(s21_bigDecimal *value, unsigned char bit);
void set_big_bit_at_position(s21_bigDecimal *value, int bit_id, int value_bit);
void update_big_decimal_properties(s21_bigDecimal *big_decimal);
void print_big_decimal_info(s21_bigDecimal *big);
void print_big_decimal_bits(s21_bigDecimal *value);

int add_big_mantissas(s21_bigDecimal *value1, s21_bigDecimal *value2,
                      s21_bigDecimal *result);
void subtract_big_mantissas(s21_bigDecimal value_1, s21_bigDecimal value_2,
                            s21_bigDecimal *result);
void add_or_subtract_big_decimals(s21_bigDecimal *value_1,
                                  s21_bigDecimal *value_2,
                                  s21_bigDecimal *result, int same_sign);

int multiply_big_mantissas(s21_bigDecimal value_1, s21_bigDecimal *value_2,
                           s21_bigDecimal *result);
void divide_big_decimals(s21_bigDecimal dividend, s21_bigDecimal divisor,
                         s21_bigDecimal *quotient);
unsigned int divide_big_mantissa_by_10(s21_bigDecimal dividend,
                                       s21_bigDecimal *result);

void normalize_scales(s21_bigDecimal *value1, s21_bigDecimal *value2);
int multiply_big_decimal_by_10(s21_bigDecimal *value, int times);

int shift_big_left(s21_bigDecimal *value, int positions);

int compare_big_mantissas(s21_bigDecimal *value1, s21_bigDecimal *value2);
int is_big_decimal_zero(s21_bigDecimal big);
int is_big_decimal_greater(s21_bigDecimal value_1, s21_bigDecimal value_2);
int is_big_decimal_greater_or_equal(s21_bigDecimal value_1,
                                    s21_bigDecimal value_2);

void apply_banker_rounding(s21_bigDecimal original, s21_bigDecimal *rounded,
                           int scale_difference);
int check_big_decimal_overflow(s21_bigDecimal *big);

void subtract_uint_from_big(s21_bigDecimal *value, unsigned int x);
int is_big_greater_or_equal_uint(const s21_bigDecimal *value, unsigned int x);
unsigned int convert_big_to_uint(s21_bigDecimal value);
unsigned int divide_by_10_with_remainder(s21_bigDecimal *value,
                                         s21_bigDecimal *quotient,
                                         unsigned int *remainder);
void transfer_bits_to_decimal(s21_bigDecimal *big_result, s21_decimal *result);

int compare_decimals(const s21_decimal *x1, const s21_decimal *x2);
int s21_is_less(s21_decimal x1, s21_decimal x2);
int s21_is_less_or_equal(s21_decimal x1, s21_decimal x2);
int s21_is_greater(s21_decimal x1, s21_decimal x2);
int s21_is_greater_or_equal(s21_decimal x1, s21_decimal x2);
int s21_is_equal(s21_decimal x1, s21_decimal x2);
int s21_is_not_equal(s21_decimal x1, s21_decimal x2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
void s21_get_float_part(char *part, int *scale, int *length, int *decimal_part);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

void full_decimal(s21_decimal *num);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
#endif