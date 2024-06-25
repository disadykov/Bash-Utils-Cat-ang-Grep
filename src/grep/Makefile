CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC = s21_grep.c
SRCLIB = func_grep.c
OBJ = s21_grep.o
OBJLIB = func_grep.o

EXEC = s21_grep

all: check s21_grep

rebuild: clean all

clean_obj:
	rm *.o

clean_execs:
	rm $(EXEC)

clean_clang:
	rm .clang-format

clean: clean_execs clean_clang clean_obj

s21_grep: $(OBJ) $(OBJLIB)
	$(CC) $(CFLAGS) $(OBJ) $(OBJLIB) -o $(EXEC)

s21_grep.o: 
	$(CC) $(CFLAGS) -c -o $(OBJ) $(SRC)

func_grep.o:
	$(CC) $(CFLAGS) -c -o $(OBJLIB) $(SRCLIB)

check:
	clear
	cp clang-format .clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
	clang-format -style=Google -i *.c
	clang-format -style=Google -i *.h
	clang-format -style=Google -n *.c
	clang-format -style=Google -n *.h

test_grep:
	bash linux_test_grep.sh

test_valgrind:
	bash linux_valgrind.sh
