CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC = s21_cat.c
SRCLIB = args_flags.c
OBJ = s21_cat.o
OBJLIB = args_flags.o

EXEC = s21_cat

all: check s21_cat

rebuild: clean all

clean_obj:
	rm *.o

clean_execs:
	rm $(EXEC)

clean_clang:
	rm .clang-format

clean: clean_execs clean_clang clean_obj

s21_cat: $(OBJ) $(OBJLIB)
	$(CC) $(CFLAGS) $(OBJ) $(OBJLIB) -o $(EXEC)

s21_cat.o: 
	$(CC) $(CFLAGS) -c -o $(OBJ) $(SRC)

args_flags.o:
	$(CC) $(CFLAGS) -c -o $(OBJLIB) $(SRCLIB)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
	clang-format -i *.c
	clang-format -i *.h
	clang-format -n *.c
	clang-format -n *.h

test_cat:
	bash linux_test_cat.sh

test_valgrind:
	bash linux_valgrind.sh
