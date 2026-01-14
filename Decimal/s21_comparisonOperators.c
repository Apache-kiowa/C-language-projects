#include "s21_decimal.h"

int compare_decimals(const s21_decimal *left, const s21_decimal *right) {
  int result = 0;
  int left_sign = get_bit_at_position(*left, 127);
  int right_sign = get_bit_at_position(*right, 127);
  if (left_sign != right_sign) {
    if (!is_decimal_zero(*left) || !is_decimal_zero(*right))
      result = left_sign ? -1 : 1;
  } else {
    s21_bigDecimal left_big = {0}, right_big = {0};
    convert_decimal_to_big(*left, &left_big);
    convert_decimal_to_big(*right, &right_big);
    normalize_scales(&left_big, &right_big);
    if (left_big.highest_bit_position != right_big.highest_bit_position)
      result = (left_big.highest_bit_position > right_big.highest_bit_position)
                   ? 1
                   : -1;
    else
      for (int bit = left_big.highest_bit_position; bit >= 0 && result == 0;
           bit--)
        result = get_bit_at_positionBig(&left_big, bit) -
                 get_bit_at_positionBig(&right_big, bit);

    if (left_sign && result) result = -result;
  }

  return (result > 0) - (result < 0);
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  return compare_decimals(&b, &a) == 1;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return compare_decimals(&b, &a) != -1;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  return compare_decimals(&a, &b) == 1;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return compare_decimals(&a, &b) != -1;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  return compare_decimals(&a, &b) == 0;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return compare_decimals(&a, &b) != 0;
}
