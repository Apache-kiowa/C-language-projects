#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res_s = s21_NULL;
  if (src != s21_NULL && str != s21_NULL && s21_strlen(src) >= start_index) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    res_s = calloc(src_len + str_len + 1, sizeof(char));
    if (res_s != s21_NULL) {
      s21_strncpy(res_s, src, start_index);
      res_s[start_index] = '\0';
      s21_strcat(res_s, str);
      s21_strcat(res_s, src + start_index);
    }
  }

  return res_s;
}
