CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11
TEST_FLAGS = -lcheck -lpthread -lm -lrt -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

LIB_SRC = s21_decimal.c s21_arithmeticOperators.c s21_comparisonOperators.c s21_converters.c s21_other.c utils.c
LIB_OBJ = $(LIB_SRC:.c=.o)
LIB = s21_decimal.a

TEST_SRC = tests.c test/*.c
TEST_BIN = test_runner

OS := $(shell uname -s)
USERNAME := $(shell whoami)

ifeq ($(OS),Darwin)
	OPEN_CMD = open
else
	OPEN_CMD = xdg-open
endif

all: $(LIB) test

s21_decimal.a: 
	${CC} ${CFLAGS} -c s21_decimal.c -o s21_decimal.o
	${CC} ${CFLAGS} -c s21_arithmeticOperators.c -o s21_arithmeticOperators.o
	${CC} ${CFLAGS} -c s21_comparisonOperators.c -o s21_comparisonOperators.o
	${CC} ${CFLAGS} -c s21_converters.c -o s21_converters.o
	${CC} ${CFLAGS} -c s21_other.c -o s21_other.o
	${CC} ${CFLAGS} -c utils.c -o utils.o
	ar rcs s21_decimal.a s21_decimal.o s21_arithmeticOperators.o s21_comparisonOperators.o s21_converters.o s21_other.o utils.o
	rm -f *.o

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(LIB_SRC)
	ar rcs s21_decimal_gcov.a $(LIB_OBJ)

	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) s21_decimal_gcov.a -o $(TEST_BIN) $(TEST_FLAGS)

	./$(TEST_BIN)

	gcov $(LIB_SRC)
	lcov -t "s21_decimal" -o s21_decimal.info -c -d .
	genhtml -o report s21_decimal.info

	rm -f $(OBJ) $(LIB) $(TEST_BIN) *.o *.gcda *.gcno *.gcov *.info *.a

	$(OPEN_CMD) report/index.html

test: $(LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -o $(TEST_BIN) $(TEST_FLAGS)
	./$(TEST_BIN)
	rm -f $(TEST_BIN) $(LIB)
	
valgrind_test: $(LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -o $(TEST_BIN) $(TEST_FLAGS)
	valgrind --tool=memcheck \
	         --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         ./$(TEST_BIN)
	rm -f $(TEST_BIN) $(LIB)


clean:
	rm -f $(OBJ) $(LIB) $(TEST_BIN) *.o *.gcda *.gcno *.gcov *.info *.a
	rm -rf report

	