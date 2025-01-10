
#include "grep.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Missing arguments\n");
  } else {
    Flags flags = {0};
    char *template = NULL;
    int fileId = flagProcessing(argc, argv, &flags, &template);

    if (fileId < 1 && flags.error && template == NULL) {
      fprintf(stderr, "./grep [flag] [pattern] [file]\n");

    } else {
      while (fileId < argc) {
        flags.currentFileNumber = fileId;
        search(&template, &flags, argv[fileId]);
        fileId++;
      }
    }
    free(template);
  }

  return 0;
}

int flagProcessing(int argc, char *argv[], Flags *flags, char **template) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:f:ivclnhso", NULL, NULL)) != -1) {
    switch (opt) {
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;

      case 'o':
        flags->o = 1;
        break;
      case 'f':
        templateFromFile(template, optarg, flags);
        break;
      case 'e':
        specifyingTemplate(template, optarg, flags);
        break;
    }
  }
  // если не сработал флаг E и F создаём шаблон по дефолту
  if ((flags->e || flags->f) == 0) {
    createTemplate(template, argv[optind]);
    optind += 1;
  }
  // если o = 1 и один из l,v,c = 1 то o = 0

  if (flags->o && (flags->l || flags->v || flags->c)) {
    if (flags->v) {
      flags->noVO = 1;
    }
    flags->o = 0;
  }

  flags->numFile = argc - optind;  // количество файлов
  if (optind >= argc || argv[optind][0] == '-' || strlen(argv[optind]) == 0) {
    flags->error = 1;
  }
  return optind;
}
void search(char **template, Flags *flags, char *fileName) {
  FILE *file = fopen(fileName, "r");
  if (file != NULL) {
    char c = '\0';
    int matchesСurrentFile = 0;

    while ((c = getc(file)) != EOF) {
      int capacity = 100;
      int index = 0;
      char *line = calloc(capacity, 1);
      flags->numberOfLines += 1;
      while (c != EOF && c != '\n') {
        if (index >= capacity - 1) {
          capacity += 100;
          line = realloc(line, capacity);
        }
        line[index++] = c;
        c = getc(file);
      }
      line[index] = '\0';
      matchesСurrentFile += parsing(template, flags, fileName, line);
      free(line);
    }

    if (flags->c && !flags->l) {
      if (flags->numFile > 1) {
        printf("%s:%d\n", fileName, matchesСurrentFile);
      } else {
        printf("%d\n", matchesСurrentFile);
      }
    }
    if (flags->l && matchesСurrentFile > 0) {
      printf("%s\n", fileName);
    }

    flags->numberOfLines = 0;
    fclose(file);
  } else if (!flags->s) {
    fprintf(stderr, "grep: %s: Нет такого файла или каталога\n", fileName);
  }
}

int parsing(char **template, Flags *flags, char *fileName, char *line) {
  regex_t regex;
  int reg = REG_EXTENDED | (flags->i ? REG_ICASE : 0);

  if (regcomp(&regex, *template, reg) != 0 && !flags->s) {
    printf("Ошибка компиляции регулярного выражения\n");
    exit(1);
  }
  int matchesСurrentFile = 0;
  int reg_v = regexec(&regex, line, 0, NULL, 0);
  int matchCondition = (reg_v == 0 && !flags->v) || (reg_v != 0 && flags->v);
  if (matchCondition) {
    matchesСurrentFile++;
  }
  if (matchCondition && !flags->noVO) {
    flags->numberMatches += (reg_v == 0 && !flags->v);
    if (!flags->l && !flags->c) {
      if (flags->numFile > 1 && !flags->l && !flags->h &&
          matchesСurrentFile > 0) {
        printf("%s:", fileName);
      }
      if (flags->n && !flags->o) printf("%d:", flags->numberOfLines);
      if (!flags->o || (reg_v != 0 && flags->v)) printf("%s\n", line);
    }
  }
  if (flags->o) flag_O(regex, line, *flags);

  regfree(&regex);
  return matchesСurrentFile;
}

void flag_O(regex_t regex, char *line, Flags flags) {
  regmatch_t matches[1];

  while (regexec(&regex, line, 1, matches, 0) == 0) {
    if (flags.n) {
      printf("%d:", flags.numberOfLines);
    }
    // Если совпадение найдено, выводим его
    int start = matches[0].rm_so;
    int end = matches[0].rm_eo;

    // Делаем подстроку из совпавшей части
    char match_str[end - start + 1];  // Массив для подстроки
    strncpy(match_str, line + start, end - start);
    match_str[end - start] = '\0';  // Завершающий ноль

    printf("%s\n", match_str);

    // Сдвигаем указатель на строку за конец текущего совпадения
    line += end;
  }
}

void specifyingTemplate(char **template, char *optarg, Flags *flags) {
  if (optarg && !flags->e && !flags->f) {
    flags->e = createTemplate(template, optarg);
    if (!flags->e && !flags->s) {
      fprintf(stderr, "Ошибка: не удалось создать шаблон.\n");
    }
  } else if (optarg && flags->e) {
    *template = realloc(*template, strlen(*template) + strlen(optarg) + 2);
    strcat(*template, "|");
    strcat(*template, optarg);
  } else if (!flags->s) {
    fprintf(stderr, "Ошибка: строка шаблона не задана.\n");
  }
}

void templateFromFile(char **template, char *optarg, Flags *flags) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    char c;
    while ((c = getc(file)) != EOF) {
      int capacity = 100;
      int index = 0;
      char *line = calloc(capacity, 1);
      if (line == NULL && !flags->s) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        break;
      }
      while (c != EOF && c != '\n') {
        line[index++] = c;
        if (index >= capacity - 1) {
          capacity += 100;
          line = realloc(line, capacity);
        }
        c = getc(file);
      }

      line[index] = '\0';  // Завершаем строку

      if (flags->e || flags->f) {
        *template = realloc(*template, strlen(*template) + strlen(line) + 2);
        strcat(*template, "|");
        strcat(*template, line);
      } else {
        flags->f = createTemplate(template, line);
      }

      free(line);
    }
    fclose(file);
  } else if (flags->s == 0) {
    fprintf(stderr, "grep: %s: Нет такого файла или каталога\n", optarg);
  }
}

int createTemplate(char **template, char *elem) {
  char *newElem = calloc(strlen(elem) + 1, sizeof(char));
  if (newElem == NULL) {
    fprintf(stderr, "Ошибка выделения памяти\n");
    exit(1);
  }

  strcpy(newElem, elem);
  *template = newElem;
  return *template != NULL ? 1 : 0;
}
