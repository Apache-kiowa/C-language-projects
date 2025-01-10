#include "cat.h"

int main(int argc, char *argv[]) {
  Arguments arguments = {0};
  int indexFilesId = 1;
  argumentProcessing(argc, argv, &arguments, &indexFilesId);
  textOutput(argc, argv, arguments, indexFilesId);
  return 0;
}

// Печать номера строки
void printLineNumber(int *number, int currentChar, Arguments arguments,
                     char last) {
  if (arguments.argument_n && last == '\n' && !arguments.argument_b) {
    printf("%6d\t", ++(*number));
  } else if (arguments.argument_b && currentChar != '\n') {
    printf("%6d\t", ++(*number));
  }
}

// Обработка символов управления (табуляции и невидимых символов)
void handleControlCharacters(int *currentChar, Arguments arguments) {
  if (arguments.argument_t && *currentChar == '\t') {
    printf("^");
    *currentChar += 64;
  }

  if (arguments.argument_e && *currentChar == '\n') {
    printf("$");
  }

  if (arguments.argument_v && *currentChar != '\t' && *currentChar != '\n') {
    if (*currentChar > 127) {
      printf("M-");
      *currentChar -= 128;
    }
    if (*currentChar < 32 || *currentChar == 127) {
      printf("^");
      *currentChar =
          *currentChar == 127 ? *currentChar - 128 + 64 : *currentChar + 64;
    }
  }
}

// Основная функция обработки файла
void processFile(FILE *file, Arguments arguments, int *number) {
  int last = '\n';
  int currentChar;
  int counterConsecutive = 0;

  while ((currentChar = fgetc(file)) != EOF) {
    // Пропуск лишних пустых строк
    if (currentChar == '\n' && last == '\n') {
      counterConsecutive++;
    } else {
      counterConsecutive = 0;
    }
    if (arguments.argument_s && counterConsecutive > 1) {
      continue;
    }

    if (last == '\n') {
      printLineNumber(number, currentChar, arguments, last);
    }

    // Обработка символов управления
    handleControlCharacters(&currentChar, arguments);

    // Печать текущего символа
    printf("%c", currentChar);
    last = currentChar;
  }
}

void textOutput(int argc, char *argv[], Arguments arguments, int indexFilesId) {
  int number = 0;
  for (int i = indexFilesId; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL) {
      fprintf(stderr, "Ошибка %s\n", argv[i]);
      continue;
    }

    processFile(file, arguments, &number);
    fclose(file);
  }
}

void argumentProcessing(int argc, char *argv[], Arguments *arguments,
                        int *indexFilesId) {
  int opt;
  while ((opt = getopt_long(argc, argv, "benstvET", opts, NULL)) != -1) {
    switch (opt) {
      case 'b':
        arguments->argument_b = 1;
        break;
      case 'e':
        arguments->argument_e = 1;
        arguments->argument_v = 1;
        break;
      case 'E':
        arguments->argument_e = 1;

        break;
      case 'n':
        arguments->argument_n = 1;
        break;
      case 's':
        arguments->argument_s = 1;
        break;
      case 't':
        arguments->argument_t = 1;
        arguments->argument_v = 1;
        break;
      case 'T':
        arguments->argument_t = 1;
        break;
      case 'v':
        arguments->argument_v = 1;
        break;
    }
  }
  *indexFilesId = optind;
}
