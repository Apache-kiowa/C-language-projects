#include "../s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *res_s = s21_NULL;

  if (src != s21_NULL && trim_chars != s21_NULL) {
    s21_size_t str_len = s21_strlen(src);

    int start_index = 0;
    int trimmed_length = (int)str_len;

    while (start_index < (int)str_len &&
           s21_strchr(trim_chars, src[start_index]) != s21_NULL) {
      start_index++;
      trimmed_length--;
    }

    int end_index = (int)str_len - 1;
    while (end_index >= start_index &&
           s21_strchr(trim_chars, src[end_index]) != s21_NULL) {
      end_index--;
      trimmed_length--;
    }

    if (trimmed_length < 0) {
      trimmed_length = 0;
    }

    char *temp = (char *)malloc(sizeof(char) * (trimmed_length + 1));
    if (temp != s21_NULL) {
      s21_memcpy(temp, src + start_index, trimmed_length);
      temp[trimmed_length] = '\0';
      res_s = temp;
    }
  }

  return res_s;
}
