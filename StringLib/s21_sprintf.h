#ifndef MY_SPRINTF
#define MY_SPRINTF

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

typedef struct {
  int minus, plus, space;
  int alignment;
  int sharp;
  int zero;
  int num;
  int star;
  int numPoint;
  int starPoint;
  char modif;
  char specfi;
  int accuracy;
  int g;
  int minusNumG;
  int numPointCheck;
  int alignmentCheck;
} Flags;

int s21_sprintf(char *buffer, const char *format, ...);
int toScientific(long double *num);
int parsingFormat(char *buffer, const char *format, va_list args);
char checkSpec(char **buffer, const char **format, va_list args,
               const char *buf2);
int parseFlags(const char **format, Flags *flags, va_list args);
int alignment(const char **format);
void checkSpace(const char **format);
int transformationSpec(char **buffer, const char **format, va_list args,
                       Flags flags, const char *buf2);
int countDigits(long long num);
void leveling(char **buffer, Flags flags, int count);
void convertToString(unsigned long long num, char **buffer);
void parsSpecInt(char **buffer, Flags flags, va_list args);
void modifikatorPars(long long *num, Flags flags, va_list args);
void parsingfeEgG(char **buf, Flags flags, va_list args);
void specifierDefinition(int *exp, int *specG, const char *specfi,
                         long double *expNum);
int checkNanInf(char *str, long double value);
void parsingfloat(char **buf, long double value, Flags flags, int specG);
void printToStr(char *str, long double value, Flags flags);
int print(char *str, long double num);
int numberOfSymbolsInFloat(long double value);
void point(char **str);
long double decimalPartFloat(char **str, long double fractionalPart,
                             int *precision);
void flagsHendler(char **buf, const char *str, long double value, Flags flags,
                  int flag);
int setSign(char **buf, long double value, Flags flags);
void setZeroes(char **buf, Flags flags);
void printNum(char **buf, const char *str);
void setSpace(char **buf, Flags flags);
void parsingEe(char **buf, long double value, Flags flags, int specG, int exp,
               long double expNum);
void printEe(char *str1, Flags flags, int exp);
void removeZero(char *str, Flags flags);
void parsingGg(char **buf, long double value, Flags flags, int specG, int exp,
               long double expNum);
void parsingString(Flags flags, char **buffer, va_list args);
void parsingUXxo(Flags flags, char **buffer, va_list args);
void unsignedModifikatorPars(unsigned long long *num, Flags flags,
                             va_list args);
void parsingU(Flags flags, char **buffer, unsigned long long num);
void int_to_base(unsigned long long num, int base, char *buffer, Flags flags);
void parsingXx(Flags flags, char **buffer, unsigned long long num);
void print0x(Flags flags, char *buffer);
void printZeroPoint(Flags flags, char *buffer, int currentLen);
void parsingO(Flags flags, char **buffer, unsigned long long num);
void parsingP(Flags flags, char **buffer, va_list args);
#endif
