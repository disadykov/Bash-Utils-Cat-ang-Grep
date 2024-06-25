#include "args_flags.h"

bool CatNoArgs(int fd, char *name) {
  bool no_error = true;

  if (fd == -1) {
    perror(name);
    no_error = false;
  } else {
    int bytes_read;
    char buf[4096];
    bytes_read = read(fd, buf, 4096);
    while (bytes_read > 0) {
      printf("%.*s", bytes_read, buf);
      bytes_read = read(fd, buf, 4096);
    }
  }
  return no_error;
}

bool FlagsParser(CatFlags *info, char *argv, char *name) {
  bool no_error = true;
  ++argv;
  if (*argv == '-') {
    ++argv;
    if (strcmp(argv, "number-nonblank") == 0) {
      info->num_nonblank = true;
    } else if (strcmp(argv, "number") == 0) {
      info->num_all = true;
    } else if (strcmp(argv, "squeeze-blank") == 0) {
      info->sq_blank = true;
    } else if (strcmp(argv, "") == 0) {
      no_error = true;
    }

    else {
      dprintf(STDERR_FILENO, "%s: invalid option '--%s'\n", name, argv);
      no_error = false;
    }
  } else {
    for (char *it = argv; *it != '\0'; ++it) {
      switch (*it) {
        case 'b':
          info->num_nonblank = true;
          break;
        case 'n':
          info->num_all = true;
          break;
        case 's':
          info->sq_blank = true;
          break;
        case 'v':
          info->show_nonprint = true;
          break;
        case 'E':
          info->show_ends = true;
          break;
        case 'e':
          info->show_ends = true;
          info->show_nonprint = true;
          break;
        case 'T':
          info->show_tabs = true;
          break;
        case 't':
          info->show_tabs = true;
          info->show_nonprint = true;
          break;
        default:
          dprintf(STDERR_FILENO, "%s: %s '%s'\n", name, "invlalid option --",
                  argv);
          return false;
      }
    }
  }
  return no_error;
}

void FlagV(int c) {
  if (c > 127) {
    c = c - 128;
    printf("M-");
  }
  if (c >= 0 && c <= 8) {
    printf("^%c", c + 64);
  } else if (c >= 11 && c <= 31) {
    printf("^%c", c + 64);
  } else if (c == 127) {
    printf("^%c", c - 64);
  } else if (c > 127 && c < 254) {
    printf("M-%c", c - 128);
  } else {
    printf("%c", c);
  }
}

bool CatArgs(CatFlags *info, int *FileNameSave, char *argv[], int count) {
  bool no_error = true;
  if ((info->file_count) > 0) {
    int lineNo = 1;
    int c = 0;
    int last = '\n';
    for (int i = 0; i < count; i++) {
      FILE *file = fopen(argv[FileNameSave[i]], "rb");
      if (errno) {
        fprintf(stderr, "%s: ", argv[0]);
        perror(argv[FileNameSave[i]]);
        free(FileNameSave);
        exit(1);
      } else {
        bool squeeze = false;
        while (fread(&c, 1, 1, file) > 0) {
          if (last == '\n') {
            if ((info->sq_blank) && (c == '\n')) {
              if (squeeze) continue;
              squeeze = true;
            } else
              squeeze = false;
            if (info->num_nonblank) {
              if (c != '\n') printf("%6i\t", lineNo++);
            } else if (info->num_all) {
              printf("%6i\t", lineNo++);
            }
          }
          if ((info->show_tabs) && (c == '\t'))
            printf("%s", "^I");
          else {
            if ((info->show_ends) && (c == '\n'))
              printf("%s", "$\n");
            else {
              if (info->show_nonprint) {
                FlagV(c);
              } else
                printf("%c", c);
            }
          }

          last = c;
        }
      }

      fclose(file);
    }
  } else {
    no_error = false;
  }
  return no_error;
}