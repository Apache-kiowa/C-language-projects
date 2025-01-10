CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g
TEST_PATH=testing

all: cat

cat:
	$(CC) $(CFLAGS) cat.c -o cat

rebuild: clean cat

clean:
	rm -rf cat
