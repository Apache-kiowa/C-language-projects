#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *result = s21_NULL;

  while (*str1 != '\0' && result == s21_NULL) {
    const char *s2 = str2;
    while (*s2 != '\0' && result == s21_NULL) {
      if (*str1 == *s2) {
        result = (char *)str1;
      }
      s2++;
    }
    str1++;
  }

  return result;
}
