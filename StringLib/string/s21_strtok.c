#include "../s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *cur_position_tokenization;
  char *res_position = s21_NULL;
  char *delimiter_position;

  if (str != s21_NULL) {
    cur_position_tokenization = str;
  }

  if (cur_position_tokenization != s21_NULL) {
    while (*cur_position_tokenization != '\0' &&
           s21_strchr(delim, *cur_position_tokenization)) {
      cur_position_tokenization++;
    }

    if (*cur_position_tokenization != '\0') {
      res_position = cur_position_tokenization;
      delimiter_position = res_position + s21_strcspn(res_position, delim);

      if (*delimiter_position != '\0') {
        *delimiter_position = '\0';
        delimiter_position++;
      }

      cur_position_tokenization = delimiter_position;
    }
  }

  return res_position;
}
