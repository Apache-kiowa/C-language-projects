#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *p = dest;
  while (*p != '\0') {
    p++;
  }
  while (n-- > 0 && *src != '\0') {
    *p++ = *src++;
  }
  *p = '\0';
  return dest;
}