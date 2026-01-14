#include "../s21_string.h"
char *s21_strstr(const char *haystack, const char *needle) {
  const char *result = s21_NULL;
  if (*needle == '\0') {
    result = haystack;
  }
  while (*haystack != '\0' && result == s21_NULL) {
    if (*haystack == *needle) {
      const char *hay = haystack;
      const char *need = needle;
      while (*need != '\0' && *hay == *need) {
        hay++;
        need++;
      }
      if (*need == '\0') {
        result = haystack;
      }
    }
    haystack++;
  }

  return (char *)result;
}
