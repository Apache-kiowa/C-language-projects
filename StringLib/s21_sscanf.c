#include "s21_sscanf.h"

unsigned long long convert_str_to_ull(const char *input, char **end_pos,
                                      int numeral_base) {
  const char *ptr = input;
  unsigned long long result = 0;
  int sign = 1;
  while (IS_SPACE((unsigned char)*ptr)) ptr++;
  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }
  if (ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X')) {
    ptr += 2;
  }
  int is_valid = 1;
  while (is_valid) {
    int digit;
    if (IS_DIGIT(*ptr)) {
      digit = *ptr - '0';
    } else if (*ptr >= 'a' && *ptr <= 'f') {
      digit = *ptr - 'a' + 10;
    } else if (*ptr >= 'A' && *ptr <= 'F') {
      digit = *ptr - 'A' + 10;
    } else {
      is_valid = 0;
      continue;
    }
    if (digit >= numeral_base) {
      is_valid = 0;
      continue;
    }
    result = result * numeral_base + digit;
    ptr++;
  }
  if (end_pos != s21_NULL) *end_pos = (char *)ptr;
  return sign * result;
}

long double convert_str_to_ld(const char *input, char **end_pos) {
  const char *ptr = input;
  long double number = 0.0, scale = 1.0;
  int exp = 0, sign = 1, exp_sign = 1, digit_seen = 0;
  while (IS_SPACE(*ptr)) ptr++;
  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }
  while (IS_DIGIT(*ptr)) {
    number = number * 10.0 + (*ptr - '0');
    ptr++;
    digit_seen = 1;
  }
  if (*ptr == '.') {
    ptr++;
    long double frac = 0.0, frac_div = 1.0;
    while (IS_DIGIT(*ptr)) {
      frac = frac * 10.0 + (*ptr - '0');
      frac_div *= 10.0;
      ptr++;
      digit_seen = 1;
    }
    number += frac / frac_div;
  }
  if (*ptr == 'e' || *ptr == 'E') {
    ptr++;
    if (*ptr == '-') {
      exp_sign = -1;
      ptr++;
    } else if (*ptr == '+') {
      ptr++;
    }
    while (IS_DIGIT(*ptr)) {
      exp = exp * 10 + (*ptr - '0');
      ptr++;
    }
  }
  scale = powl(10, exp_sign * exp);
  number *= scale * sign;
  if (end_pos != s21_NULL) *end_pos = (char *)(digit_seen ? ptr : input);
  return number;
}

double convert_str_to_d(const char *input, char **end_pos) {
  const char *ptr = input;
  double number = 0.0, scale = 1.0;
  int exp = 0, sign = 1, exp_sign = 1, digit_seen = 0;
  while (IS_SPACE(*ptr)) ptr++;
  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }
  while (IS_DIGIT(*ptr)) {
    number = number * 10.0 + (*ptr - '0');
    ptr++;
    digit_seen = 1;
  }
  if (*ptr == '.') {
    ptr++;
    double frac = 0.0, frac_div = 1.0;
    while (IS_DIGIT(*ptr)) {
      frac = frac * 10.0 + (*ptr - '0');
      frac_div *= 10.0;
      ptr++;
      digit_seen = 1;
    }
    number += frac / frac_div;
  }
  if (*ptr == 'e' || *ptr == 'E') {
    ptr++;
    if (*ptr == '-') {
      exp_sign = -1;
      ptr++;
    } else if (*ptr == '+') {
      ptr++;
    }
    while (IS_DIGIT(*ptr)) {
      exp = exp * 10 + (*ptr - '0');
      ptr++;
    }
  }
  scale = pow(10, exp_sign * exp);
  number *= scale * sign;
  if (end_pos != s21_NULL) *end_pos = (char *)(digit_seen ? ptr : input);
  return number;
}

float convert_str_to_f(const char *input, char **end_pos) {
  const char *ptr = input;
  float number = 0.0f, scale = 1.0f;
  int exp = 0, sign = 1, exp_sign = 1, digit_seen = 0;
  while (IS_SPACE((unsigned char)*ptr)) ptr++;
  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }
  while (IS_DIGIT((unsigned char)*ptr)) {
    number = number * 10.0f + (*ptr - '0');
    ptr++;
    digit_seen = 1;
  }
  if (*ptr == '.') {
    ptr++;
    float frac = 0.0f, frac_div = 1.0f;
    while (IS_DIGIT((unsigned char)*ptr)) {
      frac = frac * 10.0f + (*ptr - '0');
      frac_div *= 10.0f;
      ptr++;
      digit_seen = 1;
    }
    number += frac / frac_div;
  }
  if (*ptr == 'e' || *ptr == 'E' || *ptr == 'p' || *ptr == 'P') {
    ptr++;
    if (*ptr == '-') {
      exp_sign = -1;
      ptr++;
    } else if (*ptr == '+') {
      ptr++;
    }
    while (IS_DIGIT((unsigned char)*ptr)) {
      exp = exp * 10 + (*ptr - '0');
      ptr++;
    }
  }
  scale = powf(10, exp_sign * exp);
  number *= scale * sign;
  if (end_pos != s21_NULL) *end_pos = (char *)(digit_seen ? ptr : input);
  return number;
}

static void parse_integer(const char **src, int width, int ignore, int base,
                          int size_mod, va_list *arg_list, int *out_count) {
  unsigned long long val = 0;
  int sign = 1, base_local = base, end = 0;
  parseSignBase(src, &sign, &base_local, &width);
  while (!end &&
         ((base_local == 10
               ? IS_DIGIT(**src)
               : (base_local == 8
                      ? IS_DIGIT(**src) && **src < '8'
                      : IS_DIGIT(**src) || (**src >= 'a' && **src <= 'f') ||
                            (**src >= 'A' && **src <= 'F'))) &&
          (width-- != 0))) {
    int digit_val;
    if (IS_DIGIT(**src)) {
      digit_val = *(*src)++ - '0';
    } else if (**src >= 'a' && **src <= 'f') {
      digit_val = *(*src)++ - 'a' + 10;
    } else {
      digit_val = *(*src)++ - 'A' + 10;
    }

    if (digit_val >= base_local) {
      end = 1;
    } else {
      val = val * base_local + digit_val;
    }
  }
  val *= sign;
  if (!ignore) {
    switch (size_mod) {
      case 'h':
        *va_arg(*arg_list, short *) = (short)val;
        break;
      case 'l':
        *va_arg(*arg_list, long *) = (long)val;
        break;
      case 'L':
        *va_arg(*arg_list, long long *) = (long long)val;
        break;
      default:
        *va_arg(*arg_list, int *) = (int)val;
    }
  }
  (*out_count)++;
}

void parseSignBase(const char **src, int *sign, int *base, int *width) {
  if (**src == '-') {
    *sign = -1;
    (*src) += 1;
    (*width)--;
  } else if (**src == '+') {
    (*src)++;
    (*width)--;
  }
  if (*base == 0) {
    if (**src == '0') {
      if (*(*src + 1) == 'x' || *(*src + 1) == 'X') {
        *base = 16;
        *src += 2;
        *width -= 2;
      } else {
        *base = 8;
        (*src) += 1;
        (*width)--;
      }
    } else {
      *base = 10;
    }
  } else if (*base == 16 && **src == '0' &&
             (*(*src + 1) == 'x' || *(*src + 1) == 'X')) {
    *src += 2;
    *width -= 2;
  }
}

static void parse_float(const char **src, int width, int ignore, int size_mod,
                        va_list *arg_list, int *out_count) {
  char buf[256] = {0};
  char *buf_iter = buf;
  while ((width-- != 0) && (buf_iter < buf + 255) &&
         (**src == '.' || **src == '-' || **src == '+' ||
          IS_DIGIT((unsigned char)**src) || **src == 'e' || **src == 'E')) {
    *buf_iter++ = *(*src)++;
  }
  *buf_iter = '\0';
  if (!ignore) {
    if (size_mod == 'L') {
      *va_arg(*arg_list, long double *) = convert_str_to_ld(buf, s21_NULL);
    } else if (size_mod == 'l') {
      *va_arg(*arg_list, double *) = convert_str_to_d(buf, s21_NULL);
    } else {
      *va_arg(*arg_list, float *) = convert_str_to_f(buf, s21_NULL);
    }
    (*out_count)++;
  }
}

static void handle_format(char spec, const char **src, const char *start,
                          int width, int ignore, int size_mod,
                          va_list *arg_list, int *out_count) {
  while (IS_SPACE((unsigned char)**src)) (*src)++;

  if (spec == 'c') {
    if (!ignore) {
      *va_arg(*arg_list, char *) = *(*src)++;
      (*out_count)++;
    } else {
      (*src)++;
    }
  } else if (spec == 'd' || spec == 'i') {
    parse_integer(src, width, ignore, 0, size_mod, arg_list, out_count);
  } else if (spec == 'u') {
    parse_integer(src, width, ignore, 10, size_mod, arg_list, out_count);
  } else if (spec == 'x' || spec == 'X') {
    parse_integer(src, width, ignore, 16, size_mod, arg_list, out_count);
  } else if (spec == 'o') {
    parse_integer(src, width, ignore, 8, size_mod, arg_list, out_count);
  } else if (spec == 'f' || spec == 'g' || spec == 'G' || spec == 'e' ||
             spec == 'E') {
    parse_float(src, width, ignore, size_mod, arg_list, out_count);
  } else if (spec == 's') {
    char *dest = ignore ? s21_NULL : va_arg(*arg_list, char *);
    while (**src && !IS_SPACE((unsigned char)**src) && (width-- != 0)) {
      if (!ignore) {
        *dest++ = *(*src)++;
      } else {
        (*src)++;
      }
    }
    if (!ignore) {
      *dest = '\0';
      (*out_count)++;
    }
  } else if (spec == 'p') {
    if (!ignore) {
      *va_arg(*arg_list, void **) =
          (void *)convert_str_to_ull(*src, (char **)src, 16);
      (*out_count)++;
    }
  } else if (spec == '%') {
    if (**src == '%') (*src)++;
  } else if (spec == 'n') {
    if (!ignore) {
      *va_arg(*arg_list, int *) = *src - start;
    }
  }
}

int s21_sscanf(const char *str, const char *format, ...) {
  va_list arg_list;
  va_start(arg_list, format);
  const char *fmt_ptr = format;
  const char *str_ptr = str;
  const char *start = str;
  int converted = 0;
  int stop = 0;
  while (*fmt_ptr && *str_ptr && !stop) {
    if (*fmt_ptr == '%') {
      fmt_ptr++;
      int ignore = 0;
      int width = -1;
      char size_mod = 0;
      char spec = 0;

      if (*fmt_ptr == '*') {
        ignore = 1;
        fmt_ptr++;
      }
      if (IS_DIGIT((unsigned char)*fmt_ptr)) {
        width = 0;
        while (IS_DIGIT((unsigned char)*fmt_ptr)) {
          width = width * 10 + (*fmt_ptr - '0');
          fmt_ptr++;
        }
      }
      if (*fmt_ptr == 'h' || *fmt_ptr == 'l' || *fmt_ptr == 'L') {
        size_mod = *fmt_ptr++;
      }
      spec = *fmt_ptr++;
      handle_format(spec, &str_ptr, start, width, ignore, size_mod, &arg_list,
                    &converted);
    } else {
      if (*fmt_ptr == *str_ptr) {
        fmt_ptr++;
        str_ptr++;
      } else {
        stop = 1;
      }
    }
  }
  va_end(arg_list);
  return converted;
}
