#include "s21_decimal.h"

#include <stdio.h>

void full_decimal(s21_decimal *num) {
  for (int i = 0; i < 96; i++) {
    set_bit_at_position(num, i, 1);
  }
}