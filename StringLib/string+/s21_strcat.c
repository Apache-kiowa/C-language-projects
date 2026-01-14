#include "../s21_string.h"

char *s21_strcat(char *dest, const char *src) {
  char *buf;
  buf = (char *)dest;
  while (*buf != '\0') {
    buf++;
  }
  while (*src != '\0') {
    *buf++ = *src++;
  }
  *buf = '\0';

  return dest;
}
