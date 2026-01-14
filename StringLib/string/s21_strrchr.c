#include "../s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *ptr_last_found_symbol = s21_NULL;

  if (str != s21_NULL) {
    int len = s21_strlen(str);

    for (int i = len; i >= 0; i--) {
      if (str[i] == c) {
        ptr_last_found_symbol = (char *)(str + i);

        i = -1;
      }
    }
  }
  return ptr_last_found_symbol;
}
