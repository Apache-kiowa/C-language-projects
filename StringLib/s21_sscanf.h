#ifndef ADVANCED_H
#define ADVANCED_H

#include "s21_string.h"

#define IS_DIGIT(ch) (((ch) >= '0') && ((ch) <= '9'))

#define IS_SPACE(ch)                                              \
  ((ch) == ' ' || (ch) == '\t' || (ch) == '\n' || (ch) == '\r' || \
   (ch) == '\f' || (ch) == '\v')
int s21_sscanf(const char *str, const char *format, ...);
static void handle_format(char spec, const char **src, const char *start,
                          int width, int ignore, int size_mod,
                          va_list *arg_list, int *out_count);
static void parse_float(const char **src, int width, int ignore, int size_mod,
                        va_list *arg_list, int *out_count);
void parseSignBase(const char **src, int *sign, int *base, int *width);
static void parse_integer(const char **src, int width, int ignore, int base,
                          int size_mod, va_list *arg_list, int *out_count);
float convert_str_to_f(const char *input, char **end_pos);
double convert_str_to_d(const char *input, char **end_pos);
long double convert_str_to_ld(const char *input, char **end_pos);
unsigned long long convert_str_to_ull(const char *input, char **end_pos,
                                      int numeral_base);
void parseSignBase(const char **src, int *sign, int *detected_base, int *width);
#endif
