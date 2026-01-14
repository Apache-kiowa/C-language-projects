#include "s21_decimal.h"

static int check_int_min_case(s21_bigDecimal *full) {
  int is_int_min = 0;

  if (full->highest_bit_position == 31 && full->is_negative) {
    int all_zeros = 1;
    int i = 0;

    while (i <= 30 && all_zeros) {
      if (get_bit_at_positionBig(full, i)) {
        all_zeros = 0;
      }
      i++;
    }

    is_int_min = all_zeros;
  }

  return is_int_min;
}

static void process_normal_int(s21_bigDecimal *full, int *dst) {
  for (int i = 0; i <= 30; i++) {
    *dst |= get_bit_at_positionBig(full, i) << i;
  }
  if (full->is_negative) {
    *dst = -(*dst);
  }
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = S21_ERROR;

  if (dst) {
    clear_decimal(dst);
    result = S21_OK;

    if (src == INT_MIN) {
      dst->bits[0] = 2147483648u;
      set_sign_bit(dst, 1);
    } else {
      dst->bits[0] = abs(src);
      if (src < 0) set_sign_bit(dst, 1);
    }
  }

  return result;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = S21_ERROR;

  if (dst) {
    *dst = 0;
    int exp = get_scale_from_decimal(src);
    s21_bigDecimal full = {0};

    convert_decimal_to_big(src, &full);

    for (int i = 0; i < exp; i++) {
      divide_big_mantissa_by_10(full, &full);
    }

    update_big_decimal_properties(&full);

    if (full.highest_bit_position <= 31) {
      int is_int_min = check_int_min_case(&full);

      if (is_int_min) {
        *dst = INT_MIN;
      } else {
        process_normal_int(&full, dst);
      }

      result = (exp > 28) ? S21_ERROR : S21_OK;
    }
  }

  return result;
}

static int is_float_valid(float src) {
  return !isnan(src) && !isinf(src) &&
         !(fabsf(src) > 0 && fabsf(src) < 1e-28) &&
         fabsf(src) <= 79228162514264337593543950335.f;
}

static void adjust_scale_if_needed(char *part, int *scale, int *length,
                                   int *decimal_part, float src) {
  if (*scale < 0 && abs(*scale) + *length > 28) {
    memset(part, 0, 128);
    sprintf(part, "%.*g", 29 - abs(*scale), src);
    s21_get_float_part(part, scale, length, decimal_part);
  }
}

static void set_bits_from_number(s21_decimal *dst, long long number) {
  for (int i = 0; number; i++, number >>= 1) {
    set_bit_at_position(dst, i, number & 1);
  }
}

static void set_scale_value(s21_decimal *dst, int scale, int length) {
  if (scale < 0) {
    set_scale_to_decimal(dst, abs(scale) + length);
  } else if (scale == 0) {
    set_scale_to_decimal(dst, length);
  }
}

static int multiply_by_ten_multiple(s21_decimal *dst, int times) {
  int mul_error = 0;
  s21_decimal ten = {{10, 0, 0, 0}};

  for (int i = 0; i < times && !mul_error; i++) {
    mul_error = s21_mul(*dst, ten, dst);
  }

  return mul_error;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = S21_ERROR;

  if (dst) {
    clear_decimal(dst);

    if (is_float_valid(src)) {
      char part[128] = {0};
      int scale = 0, length = 0, decimal_part = 0;

      sprintf(part, "%.7g", src);
      s21_get_float_part(part, &scale, &length, &decimal_part);

      adjust_scale_if_needed(part, &scale, &length, &decimal_part, src);

      long long number = llabs(atoll(part)) * pow(10, length) + decimal_part;
      set_bits_from_number(dst, number);

      if (src < 0) set_sign_bit(dst, 1);

      set_scale_value(dst, scale, length);

      result = S21_OK;

      if (scale > length) {
        int mul_error = multiply_by_ten_multiple(dst, scale - length);
        if (mul_error) result = S21_ERROR;
      }
    }
  }

  return result;
}

void s21_get_float_part(char *part, int *scale, int *length,
                        int *decimal_part) {
  char *dot = strchr(part, '.');
  char *exp = strchr(part, 'e');

  *scale = *length = *decimal_part = 0;

  if (exp) {
    *exp++ = '\0';
    *scale = atoi(exp);
  }

  if (dot) {
    *dot++ = '\0';
    *length = strlen(dot);
    *decimal_part = atoi(dot);
  }
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = S21_ERROR;

  if (dst && get_scale_from_decimal(src) <= 28) {
    *dst = 0.f;

    for (int i = 0; i < 96; i++) {
      if (get_bit_at_position(src, i)) {
        *dst += powf(2, i);
      }
    }

    *dst /= powf(10, get_scale_from_decimal(src));

    if (get_sign_bit(src)) *dst *= -1;

    result = S21_OK;
  }

  return result;
}