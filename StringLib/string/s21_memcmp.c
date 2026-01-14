#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *unsigned_str1 = str1;
  const unsigned char *unsigned_str2 = str2;
  int result = 0;

  for (s21_size_t i = 0; i < n && result == 0; i++) {
    if (unsigned_str1[i] != unsigned_str2[i]) {
      result = (int)(unsigned_str1[i] - unsigned_str2[i]);
    }
  }

  return result;
}
