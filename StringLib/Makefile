CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
SRC = s21_sprintf.c s21_sscanf.c $(wildcard string/*.c) $(wildcard string+/*.c)
OBJ_FILES = $(SRC:.c=.o)
HEADERS = s21_string.h s21_sprintf.h s21_sscanf.h
TESTS = Test/Test.c $(wildcard Test/s21*)
LIBS = -lcheck -lsubunit -lm
AR = ar
ARFLAGS = rcs

all: s21_string.a test cppcheck testValgrind

s21_string.a: $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TESTS)
	$(CC) $(CFLAGS) $(SRC) $(TESTS) $(LIBS) -o unit_test
	./unit_test

testValgrind: $(TESTS)
	$(CC) $(CFLAGS) $(SRC) $(TESTS) $(LIBS) -o unit_test
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./unit_test

cppcheck:
	cppcheck --enable=all --inconclusive --language=c --std=c11 --force --suppress=missingIncludeSystem $(SRC) 2> cppcheck_report.txt

gcov_report:
	$(MAKE) clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TESTS) $(LIBS) -o unit_test
	./unit_test
	lcov -t unit_test -o coverage.info -c -d .
	genhtml coverage.info -o report
	@echo "Открой report/index.html для просмотра отчёта покрытия"

clean:
	rm -f *.o string/*.o string+/*.o *.gcda *.gcno *.info s21_string.a unit_test runner test_runner
	rm -rf report cppcheck_report.txt coverage.info


