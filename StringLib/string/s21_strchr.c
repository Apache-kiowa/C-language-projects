#include "../s21_string.h"

char* s21_strchr(const char* str, int c) {
  const char* position = s21_NULL;
  s21_size_t i = 0;
  int flag = 1;
  for (i = 0; flag; i++) {
    if ((unsigned char)str[i] == c) {
      position = &str[i];
      flag = 0;
      continue;
    }
    if (str[i] == '\0') flag = 0;
  }
  return (char*)position;
}