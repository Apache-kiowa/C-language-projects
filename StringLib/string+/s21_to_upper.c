#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  char *copy_str = s21_NULL;

  if (str != s21_NULL) {
    int len = s21_strlen(str);
    copy_str = (char *)malloc((len + 1) * sizeof(char));

    if (copy_str != s21_NULL) {
      for (int i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          copy_str[i] = str[i] - 32;
        } else {
          copy_str[i] = str[i];
        }
      }
      copy_str[len] = '\0';
    }
  }

  return copy_str;
}
