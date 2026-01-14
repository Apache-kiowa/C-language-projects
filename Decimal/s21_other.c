#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int ret = 0;

  if (!result) {
    ret = 1;
  } else {
    clear_decimal(result);

    s21_bigDecimal big_val = {0};
    convert_decimal_to_big(value, &big_val);

    while (big_val.scale > 0) {
      divide_big_mantissa_by_10(big_val, &big_val);
    }

    ret = convert_big_to_decimal(&big_val, result);

    if (ret == 0 && is_decimal_zero(*result)) {
      set_sign_bit(result, 0);
    }
  }

  return ret;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = 0;
  if (!result) {
    status = 1;
  } else {
    clear_decimal(result);
    s21_decimal trunc_val = {0};
    s21_truncate(value, &trunc_val);

    s21_decimal frac = {0};
    s21_sub(value, trunc_val, &frac);

    if (get_sign_bit(value) && !is_decimal_zero(frac)) {
      s21_decimal one = {{1, 0, 0, 0}};
      set_scale_to_decimal(&one, 0);
      s21_sub(trunc_val, one, &trunc_val);
    }
    *result = trunc_val;

    if (is_decimal_zero(*result)) {
      set_sign_bit(result, 0);
    }
  }

  return status;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int ret = 0;

  if (!result) {
    ret = 1;
  } else {
    clear_decimal(result);

    int original_sign = get_sign_bit(value);

    s21_decimal trunc_val = {0};
    s21_truncate(value, &trunc_val);

    s21_decimal frac = {0};
    s21_sub(value, trunc_val, &frac);

    set_sign_bit(&frac, 0);

    if (!is_decimal_zero(frac)) {
      s21_decimal half = {{5, 0, 0, 0}};
      set_scale_to_decimal(&half, 1);

      s21_decimal one = {{1, 0, 0, 0}};
      set_scale_to_decimal(&one, 0);

      if (s21_is_greater_or_equal(frac, half)) {
        if (original_sign) {
          s21_sub(trunc_val, one, &trunc_val);
        } else {
          s21_add(trunc_val, one, &trunc_val);
        }
      }
    }

    *result = trunc_val;

    if (is_decimal_zero(*result)) {
      set_sign_bit(result, 0);
    }
  }

  return ret;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int status = 0;
  if (!result) {
    status = 1;
  } else {
    *result = value;
    toggle_sign(result);
  }
  return status;
}