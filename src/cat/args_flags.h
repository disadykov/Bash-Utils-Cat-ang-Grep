#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef SRC_CAT_ARGS_FLAGS_H_
#define SRC_CAT_ARGS_FLAGS_H_

typedef struct {
  bool num_nonblank;   // b
  bool num_all;        // n
  bool sq_blank;       // s
  bool show_nonprint;  // v
  bool show_ends;      // E
  bool show_tabs;      // T
  int file_count;

} CatFlags;

bool CatNoArgs(int fd, char *name);
bool FlagsParser(CatFlags *info, char *argv, char *name);
bool CatArgs(CatFlags *info, int *FileNameSave, char *argv[], int count);
void FlagV(int c);

#endif  // SRC_CAT_ARGS_FLAGS_H_
