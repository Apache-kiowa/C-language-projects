#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t length_before_first_match = 0;
  int flag1 = 1;
  while (*str1 && flag1) {
    int match_indicator = 0;
    const char *scanPtr = str2;
    int flag2 = 1;
    while (*scanPtr != '\0' && flag2) {
      if (*str1 == *scanPtr) {
        match_indicator = 1;
        flag2 = 0;
        continue;
      }
      scanPtr++;
    }
    if (match_indicator) {
      flag1 = 0;
      continue;
    } else {
      length_before_first_match++;
    }
    str1++;
  }

  return length_before_first_match;
}
