#include "s21_sprintf.h"

int s21_sprintf(char *buffer, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int result = parsingFormat(buffer, format, args);
  va_end(args);
  return result;
}

int parsingFormat(char *buffer, const char *format, va_list args) {
  char *buf_ptr = buffer;
  char *start = buffer;

  while (*format) {
    if (*format != '%') {
      *buf_ptr++ = *format++;
    } else if (*(format + 1) == '%') {
      *buf_ptr++ = '%';
      format += 2;
    } else {
      format++;
      checkSpec(&buf_ptr, &format, args, buffer);
    }
  }

  *buf_ptr = '\0';
  return (int)(buf_ptr - start);
}

char checkSpec(char **buffer, const char **format, va_list args,
               const char *buf2) {
  Flags flags = {0};

  int flag = 0;
  int flagNum = 0;
  const char valid_spec[] = "XxupcdsoineEfgG";
  const char valid_flag[] = "-+ #*.hlL";
  int res = 0;
  while (**format && flag == 0) {
    for (int i = 0; valid_spec[i] != '\0'; i++) {
      if (**format == valid_spec[i]) {
        transformationSpec(buffer, format, args, flags, buf2);
        flag = 1;
        continue;
      }
    }
    for (int i = 0; valid_flag[i] != '\0'; i++) {
      if (**format == valid_flag[i]) {
        parseFlags(format, &flags, args);
      }
    }
    if (**format == '0' && !flags.zero) {
      flags.zero = 1;
      (*format)++;

    } else if (**format >= '1' && **format <= '9') {
      flagNum = 1;
      flags.alignment = alignment(format);
    } else if (**format >= '0' && **format <= '9' && flagNum) {
      flagNum = 1;
      flags.alignment = alignment(format);
    }
  }

  (*format)++;
  return res;
}

int parseFlags(const char **format, Flags *flags, va_list args) {
  if (**format == '-') {
    flags->minus = 1;
    (*format)++;
    flags->alignment = alignment(format);
    flags->alignmentCheck = 1;
  } else if (**format == '+') {
    flags->plus = 1;
    (*format)++;
  } else if (**format == ' ') {
    (*format)++;
    if (!flags->space) {
      checkSpace(format);
      flags->space = 1;
    }
  } else if (**format == '#') {
    flags->sharp = 1;
    (*format)++;
  } else if (**format == 'h') {
    flags->modif = 'h';
    (*format)++;
  } else if (**format == 'l') {
    if ((*format)[1] == 'l') {
      flags->modif = 'B';
      (*format) += 2;
    } else {
      flags->modif = 'l';
      (*format)++;
    }
  } else if (**format == 'L') {
    flags->modif = 'l';
    (*format)++;
  } else if (**format == '*') {
    flags->alignment = va_arg(args, int);
    (*format)++;
    flags->alignmentCheck = 1;
  } else if (**format == '.') {
    (*format)++;
    if (**format == '*') {
      flags->numPoint = va_arg(args, int);
      (*format)++;
      flags->numPointCheck = 1;
    } else if (**format >= '0' && **format <= '9') {
      flags->numPoint = alignment(format);
      flags->numPointCheck = 1;
    } else {
      flags->numPointCheck = 1;
      flags->numPoint = 0;
    }
  }
  return 0;
}

void checkSpace(const char **format) {
  while (**format == ' ') {
    (*format)++;
  }
}
int alignment(const char **format) {
  int res = 0;
  while (**format >= '0' && **format <= '9') {
    res = res * 10 + (**format - '0');
    (*format)++;
  }
  return res;
}

void parsSpecChar(char **buffer, Flags flags, va_list args) {
  char c = (char)va_arg(args, int);
  if (flags.space) {
    **buffer = ' ';
    (*buffer)++;
  }
  if (flags.minus) {
    **buffer = c;
    (*buffer)++;
  }
  if (flags.alignment) {
    for (int i = 0; i < flags.alignment - 1; i++) {
      **buffer = ' ';
      (*buffer)++;
    }
  }
  if (!flags.minus) {
    **buffer = c;
    (*buffer)++;
  }
}

int transformationSpec(char **buffer, const char **format, va_list args,
                       Flags flags, const char *buf2) {
  flags.specfi = **format;
  switch (flags.specfi) {
    case 'c':
      parsSpecChar(buffer, flags, args);
      break;
    case 'd':
    case 'i':
      parsSpecInt(buffer, flags, args);
      break;
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G': {
      parsingfeEgG(buffer, flags, args);
      break;
    }
    case 's':
      parsingString(flags, buffer, args);
      break;
    case 'u':
    case 'x':
    case 'X':
    case 'o':
      parsingUXxo(flags, buffer, args);
      break;
    case 'p': {
      parsingP(flags, buffer, args);
      break;
    }
    case 'n': {
      int *value = va_arg(args, int *);
      *value = *buffer - buf2;
    }
  }
  return 0;
}

void modifikatorPars(long long *num, Flags flags, va_list args) {
  if (flags.modif == 'h') {
    *num = (short int)va_arg(args, int);
  } else if (flags.modif == 'l') {
    *num = va_arg(args, long int);
  } else if (flags.modif == 'B') {
    *num = va_arg(args, long long int);
  } else {
    *num = va_arg(args, int);
  }
}

void parsSpecInt(char **buffer, Flags flags, va_list args) {
  long long num;
  modifikatorPars(&num, flags, args);

  int count = countDigits(num);

  if (num < 0 || flags.space || flags.plus) {
    count++;
  }

  if (num < 0 && (!flags.alignment || flags.zero)) {
    **buffer = '-';
    (*buffer)++;
    num = -num;
  } else if (flags.plus) {
    **buffer = '+';
    (*buffer)++;
  } else if (flags.space) {
    **buffer = ' ';
    (*buffer)++;
  }

  if (!flags.minus) {
    leveling(buffer, flags, count);
  }
  if (num < 0 && !flags.zero) {
    **buffer = '-';
    (*buffer)++;
    num = -num;
  }
  convertToString(num, buffer);

  if (flags.minus) {
    leveling(buffer, flags, count);
  }
}

void leveling(char **buffer, Flags flags, int count) {
  for (int i = 0; i < flags.alignment - count; i++) {
    if (flags.zero) {
      **buffer = '0';
    } else {
      **buffer = ' ';
    }
    (*buffer)++;
  }
}
void convertToString(unsigned long long num, char **buffer) {
  char temp[50];
  int i = 0;

  if (num == 0) {
    **buffer = '0';
    (*buffer)++;
    **buffer = '\0';
    return;
  }

  while (num > 0) {
    temp[i++] = (num % 10) + '0';
    num /= 10;
  }

  while (i > 0) {
    **buffer = temp[--i];
    (*buffer)++;
  }

  **buffer = '\0';
}

int countDigits(long long num) {
  int count = 0;

  if (num == 0)
    count = 1;
  else {
    if (num < 0) num = -num;
    while (num > 0) {
      num /= 10;
      count++;
    }
  }

  return count;
}

void parsingfeEgG(char **buf, Flags flags, va_list args) {
  long double value = 0;
  int exp = 0;
  int specG = 0;
  if (flags.modif == 'l') {
    value = va_arg(args, long double);
  } else {
    value = va_arg(args, double);
  }
  long double expNum = value;
  specifierDefinition(&exp, &specG, &flags.specfi, &expNum);
  if (flags.specfi == 'f') {
    parsingfloat(buf, value, flags, specG);
  } else if (flags.specfi == 'e' || flags.specfi == 'E') {
    parsingEe(buf, value, flags, specG, exp, expNum);
  } else if (flags.specfi == 'g' || flags.specfi == 'G') {
    parsingGg(buf, value, flags, specG, exp, expNum);
  }
}

void parsingGg(char **buf, long double value, Flags flags, int specG, int exp,
               long double expNum) {
  if (flags.numPointCheck && flags.numPoint == 0) {
    flags.numPoint = 1;
  } else if (!flags.numPoint) {
    flags.numPoint = 6;
  }

  if ((fabsl(value) / powl(10, flags.numPoint) > 1) ||
      (((fabsl(value) * (powl(10, 4))) < 1) && value != 0)) {
    flags.numPoint ? flags.numPoint-- : flags.numPoint;
    parsingEe(buf, value, flags, specG, exp, expNum);
  } else {
    if (fabsl(value) < 0.1 && value != 0) {
      int j = 1;
      if (flags.numPoint == 0) {
        flags.numPoint++;
      }
      while ((fabsl(value) * (powl(10, j))) < 1) {
        flags.numPoint++;
        j++;
      }
    } else if (value != 0) {
      if (flags.numPoint == 0) {
        flags.numPoint++;
      } else {
        flags.numPoint -= numberOfSymbolsInFloat(value);
      }
    } else {
      flags.numPoint--;
    }

    parsingfloat(buf, value, flags, specG);
  }
}

void parsingEe(char **buf, long double value, Flags flags, int specG, int exp,
               long double expNum) {
  char str[1024] = {'\0'};
  char *str1 = str;

  if (!checkNanInf(str, value)) {
    printToStr(str1, expNum, flags);
    if (specG) {
      removeZero(str, flags);
    }

    printEe(str1, flags, exp);
  }
  flagsHendler(buf, str, value, flags, 1);
}

void printEe(char *str1, Flags flags, int exp) {
  str1 += s21_strlen(str1);
  if (flags.specfi == 'e' || flags.specfi == 'g') {
    *str1 = 'e';
  } else {
    *str1 = 'E';
  }
  str1++;

  if (exp < 0) {
    *str1 = '-';
    exp = -exp;
  } else {
    *str1 = '+';
  }
  str1++;

  if (exp < 10) {
    *str1++ = '0';
    *str1++ = '0' + exp;
  } else if (exp < 100) {
    *str1++ = '0' + (exp / 10);
    *str1++ = '0' + (exp % 10);
  } else {
    *str1++ = '0' + (exp / 100);
    *str1++ = '0' + ((exp / 10) % 10);
    *str1++ = '0' + (exp % 10);
  }

  *str1 = '\0';
}

int checkNanInf(char *str, long double value) {
  int flag = 0;
  if (fabsl(value) == INFINITY) {
    str[0] = 'i';
    str[1] = 'n';
    str[2] = 'f';
    str[3] = '\0';
    flag = 1;
  } else if (isnan(value)) {
    str[0] = 'n';
    str[1] = 'a';
    str[2] = 'n';
    str[3] = '\0';
    flag = 1;
  }
  return flag;
}
void removeZero(char *str, Flags flags) {
  char *dot = s21_strchr(str, '.');
  if (!dot) return;
  char *end = str + s21_strlen(str) - 1;
  if (!flags.sharp) {
    while (end > dot && *end == '0') {
      *end-- = '\0';
    }
    if (*end == '.') {
      *end = '\0';
    }
  }
  if (flags.sharp && flags.numPointCheck) {
    int current_decimals = 0;

    dot = s21_strchr(str, '.');
    if (!dot) {
      dot = str + s21_strlen(str);
      *dot++ = '.';
      *dot = '\0';
    } else {
      current_decimals = s21_strlen(dot + 1);
    }
    int needed_zeros = flags.numPoint - current_decimals;
    if (needed_zeros > 0) {
      char *append = str + s21_strlen(str);
      for (int i = 0; i < needed_zeros; i++) {
        *append++ = '0';
      }
      *append = '\0';
    }
  }
}

void parsingfloat(char **buf, long double value, Flags flags, int specG) {
  char str[1024] = {'\0'};
  char *str1 = str;

  if (!checkNanInf(str1, value)) {
    printToStr(str1, value, flags);
    if (specG) {
      removeZero(str, flags);
    }
  }
  flagsHendler(buf, str, value, flags, 1);
}

long double round_to_precision(long double value, int precision) {
  long double factor = powl(10.0L, precision);
  return roundl(value * factor) / factor;
}

void printToStr(char *str, long double value, Flags flags) {
  long double num;
  if (flags.specfi == 'e' || flags.specfi == 'E') {
    num = floor(fabsl(value));
  } else {
    num = floor(fabsl(round_to_precision(value, flags.numPoint)));
  }
  if (value < 0) {
    num = -num;
  }
  long double fractionalPart = value - num;
  int precision = 6;
  int flag = 0;
  if (flags.numPoint != 0) {
    precision = flags.numPoint;
  } else if (flags.numPointCheck) {
    num = roundl(value);

    flag = 1;
  }
  print(str, num);
  if (flag != 1) {
    point(&str);
    flags.numPoint--;
    num = decimalPartFloat(&str, fractionalPart, &precision);

    if (num == 0) {
      for (int i = 0; i < precision; i++) {
        *str = '0';
        flags.numPoint--;
        str++;
      }
    } else {
      print(str, num);
    }
  } else if (flags.sharp) {
    point(&str);
  }
}
void point(char **str) {
  while (**str) {
    (*str)++;
  }
  **str = '.';
  (*str)++;
  **str = '\0';
}

int print(char *str, long double num) {
  if (num == 0.0) {
    str[0] = '0';
    str[1] = '\0';
  } else {
    int res = 0;
    int i = 0;
    int count = 1;
    if (num < 0.0) {
      count *= -1;
    }
    res = numberOfSymbolsInFloat(num);
    i += res - 1;

    while (fabsl(num) >= 1) {
      char ost = ((int)fmodl(num, 10)) * count + 48;
      str[i] = ost;
      num /= 10;
      i--;
    }
    str[res] = '\0';
  }
  return 0;
}

long double decimalPartFloat(char **str, long double fractionalPart,
                             int *precision) {
  long double res = fabsl(fractionalPart) * powl(10, *precision);
  if (res != 0) {
    long double flag = fabsl(fractionalPart) * 10;
    while (flag < 1 && (*precision)-- > 0) {
      flag = flag * 10;
      **str = '0';
      (*str)++;
    }
  }
  if (numberOfSymbolsInFloat(roundl(res)) > numberOfSymbolsInFloat(res)) {
    res = 0;
  } else {
    res = roundl(res);
  }
  return res;
}

int numberOfSymbolsInFloat(long double value) {
  int num = 0;
  while (fabsl(value) >= 1) {
    num++;
    value /= 10;
  }
  return num;
}

void specifierDefinition(int *exp, int *specG, const char *specfi,
                         long double *expNum) {
  if ((*specfi == 'g' || *specfi == 'G' || *specfi == 'e' || *specfi == 'E') &&
      !(fabsl(*expNum) == INFINITY || isnan(*expNum))) {
    *exp = toScientific(expNum);
    if (*specfi == 'g' || *specfi == 'G') {
      *specG = 1;
    }
  }
}

int toScientific(long double *num) {
  int exponent = 0;

  if (*num == 0.0L) {
  } else {
    while (fabsl(*num) >= 10.0L) {
      *num /= 10.0L;
      exponent++;
    }

    while (fabsl(*num) < 1.0L) {
      *num *= 10.0L;
      exponent--;
    }
  }

  return exponent;
}

void flagsHendler(char **buf, const char *str, long double value, Flags flags,
                  int flag) {
  int len = s21_strlen(str);
  flags.alignment -= len;
  if (value < 0 || flags.plus || flags.space) {
    flags.alignment--;
  }

  if (flags.minus) {
    if (flag) {
      setSign(buf, value, flags);
    }
    printNum(buf, str);
    setZeroes(buf, flags);
    setSpace(buf, flags);
  } else {
    setSpace(buf, flags);
    if (flag) {
      setSign(buf, value, flags);
    }
    setZeroes(buf, flags);
    printNum(buf, str);
  }
}

void printNum(char **buf, const char *str) {
  int len = s21_strlen(str);
  for (int i = 0; i < len; i++) {
    **buf = str[i];
    (*buf)++;
  }
}
void setSpace(char **buf, Flags flags) {
  if (flags.alignment > 0 && !flags.zero) {
    for (int i = 0; i < flags.alignment; i++) {
      **buf = ' ';
      (*buf)++;
    }
  }
}
void setZeroes(char **buf, Flags flags) {
  if (flags.zero) {
    for (int i = 0; i < flags.alignment; i++) {
      **buf = '0';
      (*buf)++;
    }
  }
}

int setSign(char **buf, long double value, Flags flags) {
  if (flags.specfi != 'u') {
    if (signbit(value)) {
      **buf = '-';
      (*buf)++;
    } else if (flags.plus) {
      **buf = '+';
      (*buf)++;
    } else if (flags.space) {
      **buf = ' ';
      ;
      (*buf)++;
    }
  }
  return 0;
}

void parsingString(Flags flags, char **buffer, va_list args) {
  const char *src = va_arg(args, char *);
  if (!src) {
    src = "(null)";
  }
  int str_len = strlen(src);
  int copy_len = str_len;
  if (flags.numPointCheck && flags.numPoint < copy_len) {
    copy_len = flags.numPoint;
  }
  char temp[1024] = {0};
  s21_strncpy(temp, src, copy_len);
  flagsHendler(buffer, temp, 0, flags, 0);
}
void parsingP(Flags flags, char **buffer, va_list args) {
  if (flags.zero) {
    flags.zero = 0;
  }
  char str[1024] = {'\0'};
  long long unsigned int num =
      (long long unsigned int)va_arg(args, long long unsigned int);
  if (num != 0) {
    char strNum[1024] = {'\0'};
    int_to_base(num, 16, strNum, flags);
    s21_strcat(str, "0x");
    s21_strcat(str, strNum);
  } else {
    str[0] = '(';
    str[1] = 'n';
    str[2] = 'i';
    str[3] = 'l';
    str[4] = ')';
  }
  flagsHendler(buffer, str, 0, flags, 0);
}

void parsingUXxo(Flags flags, char **buffer, va_list args) {
  unsigned long long num = 0;
  unsignedModifikatorPars(&num, flags, args);
  if (flags.specfi == 'u') {
    parsingU(flags, buffer, num);
  } else if (flags.specfi == 'x' || flags.specfi == 'X') {
    parsingXx(flags, buffer, num);
  } else {
    parsingO(flags, buffer, num);
  }
}

void printZeroPoint(Flags flags, char *buffer, int currentLen) {
  if (flags.numPoint && flags.numPointCheck) {
    while (currentLen++ < flags.numPoint) {
      *buffer = '0';
      buffer++;
    }
  }
}
void print0x(Flags flags, char *buffer) {
  if (flags.sharp) {
    if (flags.specfi == 'x') {
      *buffer = '0';
      buffer++;
      *buffer = 'x';
    } else if (flags.specfi == 'X') {
      *buffer = '0';
      buffer++;
      *buffer = 'X';
    }
  }
}
void parsingO(Flags flags, char **buffer, unsigned long long num) {
  char str[1024] = {'\0'};
  char zeroPadding[1024] = {'\0'};
  char strNum[1024] = {'\0'};
  char prefix[10] = {'\0'};
  int_to_base(num, 8, strNum, flags);
  int len = s21_strlen(strNum);

  if (flags.numPoint == 0 && flags.numPointCheck && num == 0) {
  } else if (flags.sharp && num != 0 && !flags.zero) {
    len++;
    prefix[0] = '0';
    prefix[1] = '\0';
  } else if (flags.zero && flags.numPointCheck) {
    flags.zero = 0;
  }

  printZeroPoint(flags, zeroPadding, len);
  s21_strcat(str, prefix);
  s21_strcat(str, zeroPadding);
  s21_strcat(str, strNum);
  if (flags.numPoint == 0 && flags.numPointCheck && num == 0) {
  } else {
    if (flags.plus) {
      flags.alignment++;
    }
    flagsHendler(buffer, str, 0, flags, 0);
  }
}
void parsingXx(Flags flags, char **buffer, unsigned long long num) {
  char str[1024] = {'\0'};
  char strNum[1024] = {'\0'};
  char prefix[10] = {'\0'};
  char zeroPadding[1024] = {'\0'};
  if (flags.zero && flags.numPointCheck) {
    flags.zero = 0;
  }
  if (num == 0) {
    if (flags.numPointCheck && flags.numPoint == 0) {
    } else {
      strNum[0] = '0';
      strNum[1] = '\0';
      flags.zero = 1;
      flags.alignment = flags.numPoint;
    }
  } else {
    int_to_base(num, 16, strNum, flags);
    int len = s21_strlen(strNum);
    print0x(flags, prefix);
    if (flags.zero) {
      flags.zero = 0;
      if (flags.sharp) {
        flags.alignment -= 2;
      }
      flags.numPoint = flags.alignment;
      flags.numPointCheck = 1;
    }
    printZeroPoint(flags, zeroPadding, len);
  }
  s21_strcat(str, prefix);
  s21_strcat(str, zeroPadding);
  s21_strcat(str, strNum);
  if (flags.plus) {
    flags.alignment++;
  }
  flagsHendler(buffer, str, 0, flags, 0);
}

void int_to_base(unsigned long long num, int base, char *buffer, Flags flags) {
  const char *digits =
      (flags.specfi == 'X') ? "0123456789ABCDEF" : "0123456789abcdef";
  char temp[32];
  int i = 0;

  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  }
  if (base != 0 && base > -1) {
    while (num > 0) {
      temp[i++] = digits[num % base];
      num /= base;
    }
  }

  for (int j = 0; j < i; j++) {
    buffer[j] = temp[i - j - 1];
  }
  buffer[i] = '\0';
}

void parsingU(Flags flags, char **buffer, unsigned long long num) {
  char str[1024];
  char *ptr = str + 1023;
  *ptr = '\0';
  if (num == 0 && !flags.numPointCheck) {
    *(--ptr) = '0';
  } else {
    while (num > 0) {
      *(--ptr) = '0' + (num % 10);
      num /= 10;
    }
  }
  int len = str + 1023 - ptr;
  if (len < flags.numPoint) {
    if (flags.zero != 0) {
      printf("ERROR\n\n");
      flags.zero = 0;
    }
    while (len < flags.numPoint) {
      *(--ptr) = '0';
      len++;
    }
  }
  if (flags.space) {
    **buffer = ' ';
    (*buffer)++;
  }
  if (flags.plus) {
    flags.alignment++;
  }

  flagsHendler(buffer, ptr, len, flags, 0);
}

void unsignedModifikatorPars(unsigned long long *num, Flags flags,
                             va_list args) {
  switch (flags.modif) {
    case 'h':
      *num = (unsigned short)va_arg(args, int);
      break;
    case 'l':
      *num = va_arg(args, unsigned long);
      break;
    case 'B':
      *num = va_arg(args, unsigned long long);
      break;
    default:
      *num = va_arg(args, unsigned int);
  }
}
