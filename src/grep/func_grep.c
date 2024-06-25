#include "func_grep.h"

void *xmalloc(size_t size) {
  void *temp;
  temp = malloc(size);
  if (!temp) exit(errno);
  return temp;
}

void *xrealloc(void *block, size_t size) {
  void *temp;
  temp = realloc(block, size);
  if (!temp) exit(errno);
  return temp;
}

char *pattern_add(char *string, size_t *size, char const *expr,
                  size_t size_expr) {
  string = xrealloc(string, *size + size_expr + 7);
  string[*size] = '\\';
  string[*size + 1] = '|';
  string[*size + 2] = '\\';
  string[*size + 3] = '(';
  memcpy(string + *size + 4, expr, size_expr);
  *size += size_expr + 4;
  string[*size] = '\\';
  string[*size + 1] = ')';
  string[*size + 2] = '\0';
  *size += 2;
  return string;
}

Flags GrepFlagParser(int argc, char *argv[]) {
  Flags flags = {NULL,  0,     0,     false, false, false, false,
                 false, false, false, false, false, false, 0};

  int currentFlag = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0);
  flags.pattern = xmalloc(2);
  flags.pattern[0] = '\\';
  flags.pattern[1] = '\0';
  size_t pattern_size = 0;
  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) {
    switch (currentFlag) {
      case 'e':
        flags.pattern =
            pattern_add(flags.pattern, &pattern_size, optarg, strlen(optarg));
        flags.flag_e = true;
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        flags.flag_i = true;
        break;
      case 'v':
        flags.flag_v = true;
        break;
      case 'c':
        flags.flag_c = true;
        break;
      case 'l':
        flags.flag_l = true;
        break;
      case 'n':
        flags.flag_n = true;
        break;
      case 'h':
        flags.flag_h = true;
        break;
      case 's':
        flags.flag_s = true;
        break;
      case 'f':
        flags.flag_f = true;
        break;
      case 'o':
        flags.flag_o = true;
        break;
      default:
        break;
    }
  }
  flags.size = pattern_size;
  return flags;
}

void FlagC(FILE *file, char const *filename, regex_t *preg, int file_count) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      count++;
    }
  }
  if (count > 0) {
    if (file_count <= 2)
      printf("%d\n", count);
    else
      printf("%s:%d\n", filename, count);
  }
  free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg, char *filename) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count_line = 0;
  while (getline(&line, &length, file) > 0) {
    ++count_line;
    if (flags.flag_h) flags.file_count = 1;
    if (flags.flag_v) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.flag_o)
          ;
        else if (flags.flag_l) {
          printf("%s\n", filename);
          break;
        } else {
          if (flags.flag_n) {
            if (flags.file_count == 1) {
              printf("%i:%s", count_line, line);
              EndLine(line);
            }

            else {
              printf("%s:%i:%s", filename, count_line, line);
              EndLine(line);
            }

          } else {
            if (flags.file_count == 1) {
              printf("%s", line);
              EndLine(line);
            }

            else {
              printf("%s:%s", filename, line);
              EndLine(line);
            }
          }
        }
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (flags.flag_o) {
          if (flags.flag_l) {
            printf("%s\n", filename);
            break;
          } else {
            if (flags.flag_n)
              printf("%s:%i%.*s\n", filename, count_line,
                     (int)(match.rm_eo - match.rm_so), line + match.rm_so);
            else
              printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
            char *remaining = line + match.rm_eo;
            while (!regexec(preg, remaining, 1, &match, 0)) {
              if (flags.flag_n)
                printf("%s:%i%.*s\n", filename, count_line,
                       (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
              else
                printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
              remaining = remaining + match.rm_eo;
            }
          }

        } else {
          if (flags.flag_l) {
            printf("%s\n", filename);
            break;
          } else {
            if (flags.flag_n) {
              if (flags.file_count == 1) {
                printf("%i:%s", count_line, line);
                EndLine(line);
              }

              else {
                printf("%s:%i:%s", filename, count_line, line);
                EndLine(line);
              }

            } else {
              if (flags.file_count == 1) {
                printf("%s", line);
                EndLine(line);
              }

              else {
                printf("%s:%s", filename, line);
                EndLine(line);
              }
            }
          }
        }
      }
    }
  }
  free(line);
}
void Grep(int argc, char *argv[], Flags flags) {
  char **end = &argv[argc];
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  if (flags.size == 0) {
    if (regcomp(preg, argv[0], flags.regex_flag)) {
      fprintf(stderr, "failed to compile regex1\n");
      exit(1);
    }
  } else {
    if (regcomp(preg, flags.pattern + 2, flags.regex_flag)) {
      fprintf(stderr, "failed to compile regex2\n");
      exit(1);
    }
  }
  free(flags.pattern);
  if (argc == (flags.size ? 2 : 1)) {
    if (flags.flag_c) {
      FlagC(stdin, "", preg, 1);
    } else
      GrepFile(stdin, flags, preg, "");
  }

  for (char **filename = argv + (flags.size ? 0 : 1); filename != end;
       ++filename) {
    flags.file_count++;
  }

  for (char **filename = argv + (flags.size ? 0 : 1); filename != end;
       ++filename) {
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      if (!flags.flag_s) {
        fprintf(stderr, "%s: ", argv[0]);
        perror(*filename);
      }
      continue;
    }

    if (flags.flag_c) {
      FlagC(file, *filename, preg, argc);
    } else {
      GrepFile(file, flags, preg, *filename);
    }
    fclose(file);
  }
  regfree(preg);
}

bool EndLine(char const *str) {
  int check = 0;
  int i = 0;
  bool flag = false;
  while (str[i] != '\0') {
    if (str[i] == '\n') check++;
    i++;
  }
  if (check == 0) {
    printf("\n");
    flag = true;
  }
  return flag;
}