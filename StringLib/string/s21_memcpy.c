#include "../s21_string.h"
void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *unsigned_dest = dest;
  const unsigned char *unsigned_src = src;

  for (s21_size_t i = 0; i < n; i++) {
    unsigned_dest[i] = unsigned_src[i];
  }

  return dest;
}
