#include "../s21_string.h"
void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *unsigned_str = str;
  unsigned char unsigned_c = c;
  for (s21_size_t i = 0; i < n; i++) {
    unsigned_str[i] = unsigned_c;
  }

  return str;
}
