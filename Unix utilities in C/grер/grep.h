#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;  // -
  int i;  // -
  int v;  // -
  int c;  // -
  int l;  // -
  int n;  // -
  int h;  // -
  int s;  // -
  int f;  // -
  int o;
  int error;
  int numberOfLines;
  int numFile;
  int currentFileNumber;
  int noVO;
  int numberMatches;  // количество совпадений
} Flags;

int flagProcessing(int argc, char *argv[], Flags *flags, char **template);
void templateFromFile(char **template, char *optarg, Flags *flags);
void templateFromFile(char **template, char *filename, Flags *flags);
int createTemplate(char **template, char *elem);
void specifyingTemplate(char **template, char *optarg, Flags *flags);
void search(char **template, Flags *flags, char *fileName);
int parsing(char **template, Flags *flags, char *fileName, char *line);
void flag_O(regex_t regex, char *line, Flags flags);
#endif
// e_option: Используется для задания паттерна с помощью опции -e (позволяет
// задать несколько паттернов).

// ignore_case: Включает опцию -i, которая игнорирует регистр символов при
// поиске.

// invert_match: Включает опцию -v, которая инвертирует результат поиска
// (выводит строки, не содержащие паттерн).

// count_matches: Включает опцию -c, которая выводит количество строк,
// содержащих паттерн, вместо самих строк.

// list_files: Включает опцию -l, которая выводит только имена файлов, в которых
// найден хотя бы один совпадающий паттерн.

// show_line_numbers: Включает опцию -n, которая выводит номер строки, в которой
// найдено совпадение.

// suppress_filenames: Включает опцию -h, которая не выводит имя файла перед
// строкой, даже если обрабатывается несколько файлов.

// suppress_errors: Включает опцию -s, которая подавляет ошибки (например, если
// файл не удается открыть).

// f_option: Используется для задания паттерна с помощью опции -f, которая
// читает паттерн из файла.

// only_matching: Включает опцию -o, которая выводит только части строк, которые
// совпадают с паттерном.

// remaining_files_count: Хранит количество файлов, которые ещё предстоит
// обработать.

// line_number: Хранит номер текущей строки при обходе файла.
