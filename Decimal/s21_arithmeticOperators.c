#include "s21_decimal.h"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (result == NULL) {
    status = 1;
  } else {
    clear_decimal(result);
    s21_bigDecimal big_value_1 = {0};
    s21_bigDecimal big_value_2 = {0};
    s21_bigDecimal big_result = {0};
    convert_decimal_to_big(value_1, &big_value_1);
    convert_decimal_to_big(value_2, &big_value_2);
    normalize_scales(&big_value_1, &big_value_2);
    big_result.scale = big_value_1.scale;
    int same_sign = (get_bit_at_position(value_1, 127) ==
                     get_bit_at_position(value_2, 127));
    add_or_subtract_big_decimals(&big_value_1, &big_value_2, &big_result,
                                 same_sign);
    status = convert_big_to_decimal(&big_result, result);
  }
  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (!result) {
    status = 1;
  } else {
    clear_decimal(result);
    toggle_sign(&value_2);
    s21_bigDecimal big_1 = {0}, big_2 = {0}, big_result = {0};
    convert_decimal_to_big(value_1, &big_1);
    convert_decimal_to_big(value_2, &big_2);
    normalize_scales(&big_1, &big_2);
    int same_sign = (get_bit_at_position(value_1, 127) ==
                     get_bit_at_position(value_2, 127));
    add_or_subtract_big_decimals(&big_1, &big_2, &big_result, same_sign);
    big_result.scale = big_1.scale;
    status = convert_big_to_decimal(&big_result, result);
  }
  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (!result) {
    status = 1;
  } else {
    clear_decimal(result);
    if (!is_decimal_zero(value_1) && !is_decimal_zero(value_2)) {
      s21_bigDecimal big_1 = {0};
      s21_bigDecimal big_2 = {0};
      s21_bigDecimal big_result = {0};
      convert_decimal_to_big(value_1, &big_1);
      convert_decimal_to_big(value_2, &big_2);
      int same_sign = (get_bit_at_position(value_1, 127) ==
                       get_bit_at_position(value_2, 127));
      status = multiply_big_mantissas(big_1, &big_2, &big_result);
      if (status == 0) {
        big_result.is_negative = same_sign ? 0 : 1;
        status = convert_big_to_decimal(&big_result, result);
      }
    }
  }
  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (!result) {
    status = 1;
  } else {
    clear_decimal(result);
    if (is_decimal_zero(value_2)) {
      status = 3;
    } else if (is_decimal_zero(value_1)) {
    } else {
      s21_bigDecimal big_value_1 = {0};
      s21_bigDecimal big_value_2 = {0};
      s21_bigDecimal big_result = {0};
      convert_decimal_to_big(value_1, &big_value_1);
      convert_decimal_to_big(value_2, &big_value_2);
      divide_big_decimals(big_value_1, big_value_2, &big_result);
      big_result.is_negative = (get_bit_at_position(value_1, 127) !=
                                get_bit_at_position(value_2, 127));
      status = convert_big_to_decimal(&big_result, result);
      if (status != 0) {
        clear_decimal(result);
      }
    }
  }
  return status;
}

void add_or_subtract_big_decimals(s21_bigDecimal *value_1,
                                  s21_bigDecimal *value_2,
                                  s21_bigDecimal *result, int same_sign) {
  if (same_sign) {
    add_big_mantissas(value_1, value_2, result);
    result->is_negative = value_1->is_negative;
  } else {
    int comparison = compare_big_mantissas(value_1, value_2);
    if (comparison > 0) {
      subtract_big_mantissas(*value_1, *value_2, result);
      result->is_negative = value_1->is_negative;
    } else if (comparison < 0) {
      subtract_big_mantissas(*value_2, *value_1, result);
      result->is_negative = value_2->is_negative;
    } else {
      *result = (s21_bigDecimal){0};
    }
  }
}

void convert_decimal_to_big(s21_decimal decimal, s21_bigDecimal *big_decimal) {
  big_decimal->bits[0] = decimal.bits[0];
  big_decimal->bits[1] = decimal.bits[1];
  big_decimal->bits[2] = decimal.bits[2];
  big_decimal->scale = get_scale_from_decimal(decimal);
  big_decimal->is_negative = get_bit_at_position(decimal, 127);
  update_big_decimal_properties(big_decimal);
}

int convert_big_to_decimal(s21_bigDecimal *big, s21_decimal *dec) {
  int status = (!big || !dec);
  if (!status) {
    s21_bigDecimal t = *big;
    int rounds = 0;
    update_big_decimal_properties(&t);
    while (t.scale > 28 && !status) {
      if (!t.bits[7] && !t.bits[6] && !t.bits[5] && !t.bits[4] && !t.bits[3] &&
          !t.bits[2] && !t.bits[1] && t.bits[0] < 10) {
        clear_decimal(dec);
        status = 2;
      } else {
        divide_big_mantissa_by_10(t, &t);
        rounds++;
      }
    }
    while (!status && t.scale > 0 && check_big_decimal_overflow(&t)) {
      divide_big_mantissa_by_10(t, &t);
      rounds++;
    }
    if (!status && rounds) {
      apply_banker_rounding(*big, &t, rounds);
    }
    if (!status && check_big_decimal_overflow(&t)) {
      status = (t.scale <= 0);
      if (status && big->is_negative) status = 2;
    }
    if (!status) {
      clear_decimal(dec);
      for (int i = 0; i < 3; i++) dec->bits[i] = t.bits[i];
      set_scale_to_decimal(dec, t.scale);
      if (t.is_negative) set_sign_bit(dec, 1);
    }
  }
  return status;
}

int add_big_mantissas(s21_bigDecimal *value1, s21_bigDecimal *value2,
                      s21_bigDecimal *result) {
  int carry = 0;
  for (int i = 0; i <= 255; i++) {
    int sum = get_big_bit_at_position(value1, i) +
              get_big_bit_at_position(value2, i) + carry;
    set_big_bit_at_position(result, i, sum & 1);
    carry = sum >> 1;
  }
  return carry;
}

void subtract_big_mantissas(s21_bigDecimal value_1, s21_bigDecimal value_2,
                            s21_bigDecimal *result) {
  int borrow = 0;
  int difference = 0;
  for (int i = 0; i <= 255; i++) {
    difference = get_big_bit_at_position(&value_1, i) -
                 get_big_bit_at_position(&value_2, i) - borrow;
    borrow = difference < 0;
    difference = abs(difference);
    set_big_bit_at_position(result, i, difference % 2);
  }
}

int multiply_big_mantissas(s21_bigDecimal value_1, s21_bigDecimal *value_2,
                           s21_bigDecimal *result) {
  int status = 0;
  clear_big_decimal(result);
  update_big_decimal_properties(&value_1);
  update_big_decimal_properties(value_2);
  for (int i = 0; i <= value_2->highest_bit_position && !status; ++i) {
    if (i && (status = shift_big_left(&value_1, 1))) continue;
    if (get_big_bit_at_position(value_2, i)) {
      status = add_big_mantissas(result, &value_1, result);
    }
  }
  result->is_negative = value_1.is_negative != value_2->is_negative;
  result->scale = value_1.scale + value_2->scale;
  return status;
}

unsigned int divide_big_mantissa_by_10(s21_bigDecimal dividend,
                                       s21_bigDecimal *result) {
  s21_bigDecimal quotient = {0};
  unsigned long long carry = 0;
  for (int i = 7; i >= 0; i--) {
    unsigned long long current = (carry << 32) | dividend.bits[i];
    quotient.bits[i] = current / 10;
    carry = current % 10;
  }
  quotient.scale = dividend.scale - 1;
  quotient.is_negative = dividend.is_negative;
  *result = quotient;
  return (unsigned int)carry;
}

void divide_big_decimals(s21_bigDecimal dividend, s21_bigDecimal divisor,
                         s21_bigDecimal *quotient) {
  if (!is_big_decimal_zero(dividend)) {
    clear_big_decimal(quotient);
    return;
  }
  int scale_difference = dividend.scale - divisor.scale;
  s21_bigDecimal remainder = dividend;
  s21_bigDecimal current_quotient = {0};
  s21_bigDecimal power_value = {0};
  s21_bigDecimal next_value = {0};
  s21_bigDecimal term = {0};
  while (is_big_decimal_zero(remainder) && current_quotient.scale < 31) {
    while (is_big_decimal_greater(divisor, remainder)) {
      multiply_big_decimal_by_10(&remainder, 1);
      multiply_big_decimal_by_10(&current_quotient, 1);
    }
    int power = 0;
    power_value = next_value = divisor;
    update_big_decimal_properties(&remainder);
    int bit_difference =
        remainder.highest_bit_position - divisor.highest_bit_position;
    if (bit_difference > 2) {
      power = bit_difference - 2;
      shift_big_left(&next_value, power);
    }
    while (is_big_decimal_greater_or_equal(remainder, next_value)) {
      power_value = next_value;
      shift_big_left(&next_value, 1);
      power++;
    }
    power--;
    clear_big_decimal(&term);
    set_big_bit_at_position(&term, power, 1);
    add_big_mantissas(&current_quotient, &term, &current_quotient);
    if (is_big_decimal_greater_or_equal(remainder, power_value)) {
      subtract_big_mantissas(remainder, power_value, &remainder);
    }
  }
  current_quotient.scale += scale_difference;
  if (scale_difference < 0) {
    multiply_big_decimal_by_10(&current_quotient, -scale_difference);
  }
  *quotient = current_quotient;
}

int shift_big_left(s21_bigDecimal *value, int positions) {
  int status = 0;
  if (255 - value->highest_bit_position < positions) {
    status = 1;
  }
  for (int i = value->highest_bit_position; i >= 0; --i) {
    set_big_bit_at_position(value, i + positions,
                            get_big_bit_at_position(value, i));
  }
  for (int i = 0; i < positions; ++i) {
    set_big_bit_at_position(value, i, 0);
  }
  update_big_decimal_properties(value);
  return status;
}

int compare_big_mantissas(s21_bigDecimal *value1, s21_bigDecimal *value2) {
  int result = 0;
  for (int i = 255; i >= 0; i--) {
    int bit1 = get_big_bit_at_position(value1, i);
    int bit2 = get_big_bit_at_position(value2, i);

    if (bit1 != bit2) {
      result = bit1 - bit2;
      i = -1;
      continue;
    }
  }
  return result;
}

int is_big_decimal_zero(s21_bigDecimal big) {
  int result = 0;
  for (int i = 7; i >= 0; i--) {
    if (big.bits[i] != 0) {
      result = 1;
      i = -1;
      continue;
    }
  }
  return result;
}

int is_big_decimal_greater(s21_bigDecimal value_1, s21_bigDecimal value_2) {
  int result = 0;
  int found_difference = 0;
  for (int i = 7; i >= 0 && !result && !found_difference; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) {
        found_difference = 1;
      }
    }
  }
  return result;
}

int is_big_decimal_greater_or_equal(s21_bigDecimal value_1,
                                    s21_bigDecimal value_2) {
  int result = 1;
  for (int i = 7; i >= 0; i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      result = 1;
      i = -1;
      continue;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      result = 0;
      i = -1;
      continue;
    }
  }
  return result;
}

void normalize_scales(s21_bigDecimal *value1, s21_bigDecimal *value2) {
  int scale_difference = value1->scale - value2->scale;

  if (scale_difference > 0) {
    multiply_big_decimal_by_10(value2, scale_difference);
    update_big_decimal_properties(value2);
  } else if (scale_difference < 0) {
    multiply_big_decimal_by_10(value1, -scale_difference);
    update_big_decimal_properties(value1);
  }
}

int multiply_big_decimal_by_10(s21_bigDecimal *value, int times) {
  int status = 0;
  s21_bigDecimal ten = {0};
  ten.bits[0] = 10;

  for (int i = 1; i <= times; i++) {
    status = multiply_big_mantissas(*value, &ten, value);
  }

  value->scale += times;
  return status;
}

void apply_banker_rounding(s21_bigDecimal original, s21_bigDecimal *rounded,
                           int scale_difference) {
  if (scale_difference > 0) {
    scale_difference--;
  }
  s21_bigDecimal one = {{1}, 0, 0, 0, 0, 0};
  s21_bigDecimal half = {{5}, 1, 0, 0, 0, 0};
  s21_bigDecimal remainder = {0};
  s21_bigDecimal copy_rounded = *rounded;
  normalize_scales(&original, &copy_rounded);
  subtract_big_mantissas(original, copy_rounded, &remainder);
  remainder.is_negative = 0;
  multiply_big_decimal_by_10(&half, scale_difference);
  int comparison = compare_big_mantissas(&remainder, &half);
  if (comparison > 0 ||
      (comparison == 0 && get_big_bit_at_position(rounded, 0))) {
    add_big_mantissas(rounded, &one, rounded);
  }
}
int check_big_decimal_overflow(s21_bigDecimal *big) {
  int has_overflow = 0;
  for (int i = 3; i < 8; i++) {
    if (big->bits[i] != 0) {
      has_overflow = 1;
      i = 9;
      continue;
    }
  }
  return has_overflow;
}
