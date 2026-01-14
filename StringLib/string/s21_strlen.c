#include "../s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t str_length = 0;
  while (str[str_length] != '\0') {
    str_length++;
  }
  return str_length;
}
