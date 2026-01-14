#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *unsigned_str = str;
  unsigned char unsigned_c = c;
  void *result = s21_NULL;

  for (s21_size_t i = 0; i < n && result == s21_NULL; i++) {
    if (unsigned_str[i] == unsigned_c) {
      result = (void *)(unsigned_str + i);
    }
  }

  return result;
}
