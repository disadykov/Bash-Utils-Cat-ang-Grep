#include "func_grep.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...\n");
    fprintf(stderr, "Try 'grep --help' for more information.\n");
  } else {
    Flags flags = GrepFlagParser(argc, argv);
    argv += optind;
    argc -= optind;
    Grep(argc, argv, flags);
  }

  return 0;
}
