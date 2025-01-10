#include <getopt.h>
#include <stdio.h>

typedef struct {
  int argument_b;  // --number-nonblank
  int argument_e;  // -e
  int argument_n;  // --number
  int argument_s;  // --squeeze-blank
  int argument_t;  // -t
  int argument_v;  // -v
  int argument_e_E;
} Arguments;

void argumentProcessing(int argc, char *argv[], Arguments *arguments,
                        int *indexFilesId);
void textOutput(int argc, char *argv[], Arguments arguments, int indexFilesId);
void printLineNumber(int *number, int currentChar, Arguments arguments,
                     char last);
void handleControlCharacters(int *currentChar, Arguments arguments);
void processFile(FILE *file, Arguments arguments, int *number);

const struct option opts[] = {{"number-nonblank", 0, 0, 'b'},
                              {"number", 0, 0, 'n'},
                              {"squeeze-blank", 0, 0, 's'},
                              {0, 0, 0, 0}};
