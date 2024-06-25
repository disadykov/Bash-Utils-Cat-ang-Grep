#ifndef SRC_GREP_FUNC_GREP_H_
#define SRC_GREP_FUNC_GREP_H_

#define _GNU_SOURCE  // getline
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *pattern;
  size_t size;
  int regex_flag;  // -e -i
  bool flag_e;     // -e
  bool flag_i;     // -i
  bool flag_v;     // -v
  bool flag_c;     // -c
  bool flag_l;     // -l
  bool flag_n;     // -n
  bool flag_h;     // -h
  bool flag_s;     // -s
  bool flag_f;     // -f
  bool flag_o;     // -o
  int file_count;
} Flags;

char *pattern_add(char *string, size_t *size, char const *expr,
                  size_t size_expr);
Flags GrepFlagParser(int argc, char *argv[]);
void *xmalloc(size_t size);
void *xrealloc(void *block, size_t size);
void FlagC(FILE *file, char const *filename, regex_t *preg, int file_count);
void GrepFile(FILE *file, Flags flags, regex_t *preg, char *filename);
void Grep(int argc, char *argv[], Flags flags);
bool EndLine(char const *str);

#endif  // SRC_GREP_FUNC_GREP_H_