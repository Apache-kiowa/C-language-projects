#include "s21_decimal.h"

void clear_decimal(s21_decimal *value) {
  value->bits[0] = 0;
  value->bits[1] = 0;
  value->bits[2] = 0;
  value->bits[3] = 0;
}

int get_scale_from_decimal(s21_decimal value) {
  return (value.bits[3] & 0x00FF0000) >> 16;
}

int get_bit_at_position(s21_decimal value, unsigned char bit) {
  return (value.bits[bit / 32] >> (bit % 32)) & 1;
}
int get_bit_at_positionBig(s21_bigDecimal *value, unsigned char bit) {
  return (value->bits[bit / 32] >> (bit % 32)) & 1;
}

void set_bit_at_position(s21_decimal *num, int bit, int value) {
  if (value == 1) {
    num->bits[bit / 32] = num->bits[bit / 32] | (1 << bit % 32);
  } else if (value == 0) {
    num->bits[bit / 32] = num->bits[bit / 32] & ~(1 << bit % 32);
  }
}

int get_sign_bit(s21_decimal num) { return get_bit_at_position(num, 127); }

void set_sign_bit(s21_decimal *num, int sign) {
  set_bit_at_position(num, 127, sign);
}

void toggle_sign(s21_decimal *value) {
  set_sign_bit(value, !get_bit_at_position(*value, 127));
}

void set_scale_to_decimal(s21_decimal *dec, int scale) {
  if (!dec) return;

  dec->bits[3] &= 0xFF00FFFF;

  if (scale < 0) scale = 0;
  if (scale > 28) scale = 28;

  dec->bits[3] |= (scale & 0xFF) << 16;
}

int is_decimal_zero(s21_decimal value) {
  int res = 1;
  for (int i = 2; i >= 0; i--) {
    if (value.bits[i] != 0) {
      res = 0;
    }
  }
  return res;
}

void clear_big_decimal(s21_bigDecimal *value) {
  for (int i = 0; i < 8; i++) {
    value->bits[i] = 0;
  }
  value->scale = 0;
  value->is_negative = 0;
  value->highest_bit_position = 0;
  value->lowest_bit_position = 0;
  value->leading_zeros = 0;
}

void clear_big_mantissa(s21_bigDecimal *value) {
  for (int i = 0; i < 8; i++) {
    value->bits[i] = 0;
  }
}

int get_big_bit_at_position(s21_bigDecimal *value, unsigned char bit) {
  return (value->bits[bit / 32] >> (bit % 32)) & 1;
}

void set_big_bit_at_position(s21_bigDecimal *value, int bit_id, int value_bit) {
  if (value_bit == 1) {
    value->bits[bit_id / 32] = value->bits[bit_id / 32] | (1 << bit_id % 32);
  } else if (value_bit == 0) {
    value->bits[bit_id / 32] = value->bits[bit_id / 32] & ~(1 << bit_id % 32);
  }
}

void update_big_decimal_properties(s21_bigDecimal *big_decimal) {
  int i = 255;
  while (i >= 0 && !get_big_bit_at_position(big_decimal, i)) {
    i--;
  }

  big_decimal->highest_bit_position = i;
  big_decimal->leading_zeros = 255 - i;
  for (int j = 0; j <= 255; j++) {
    if (get_big_bit_at_position(big_decimal, j)) {
      big_decimal->lowest_bit_position = j;
      j = 256;
      continue;
    }
  }
}