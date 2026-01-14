CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11
TEST_FLAGS = -lcheck -lpthread -lm -lrt -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

LIB_SRC = s21_matrix.c matrix_convert.c matrix_mult.c matrix_sumSub.c
LIB_OBJ = $(LIB_SRC:.c=.o)
LIB = s21_matrix.a

TEST_SRC = s21_tests.c 
TEST_BIN = test_runner
# matrix_
OS := $(shell uname -s)
USERNAME := $(shell whoami)

ifeq ($(OS),Darwin)
	OPEN_CMD = open
else
	OPEN_CMD = xdg-open
endif

all: $(LIB)

s21_matrix.a: 
	${CC} ${CFLAGS} -c s21_matrix.c -o s21_matrix.o
	${CC} ${CFLAGS} -c matrix_convert.c -o matrix_convert.o
	${CC} ${CFLAGS} -c matrix_mult.c -o matrix_mult.o
	${CC} ${CFLAGS} -c matrix_sumSub.c -o matrix_sumSub.o
	${CC} ${CFLAGS} -c s21_matrix.c -o s21_matrix.o
	ar rcs s21_matrix.a s21_matrix.o matrix_convert.o matrix_mult.o matrix_sumSub.o s21_matrix.o
	rm -f *.o

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(LIB_SRC)
	ar rcs s21_matrix_gcov.a $(LIB_OBJ)

	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) s21_matrix_gcov.a -o $(TEST_BIN) $(TEST_FLAGS)

	./$(TEST_BIN)

	gcov $(LIB_SRC)
	lcov -t "s21_matrix" -o s21_matrix.info -c -d .
	genhtml -o report s21_matrix.info

	rm -f $(OBJ) $(LIB) $(TEST_BIN) *.o *.gcda *.gcno *.gcov *.info *.a *.log

	$(OPEN_CMD) report/index.html

test: clean $(LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -o $(TEST_BIN) $(TEST_FLAGS)
	./$(TEST_BIN)
	rm -f $(TEST_BIN) $(LIB) *.o *.gcda *.gcno *.gcov *.info *.a *.log
	
valgrind_test: $(LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -o $(TEST_BIN) $(TEST_FLAGS)
	valgrind --tool=memcheck \
	         --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         ./$(TEST_BIN)
	rm -f $(OBJ) $(LIB) $(TEST_BIN) *.o *.gcda *.gcno *.gcov *.info *.a *.log


clean:
	rm -f $(OBJ) $(LIB) $(TEST_BIN) *.o *.gcda *.gcno *.gcov *.info *.a *.log
	rm -rf report
