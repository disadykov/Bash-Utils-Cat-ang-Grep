#include "args_flags.h"

int main(int argc, char *argv[]) {
  int *FileNameSave = NULL;
  CatFlags info = {0, 0, 0, 0, 0, 0, 0};
  if (argc == 1) {
    CatNoArgs(STDIN_FILENO, argv[0]);
  } else {
    bool error_check = true;
    int FileIndexArgc = 0;
    int *OldBuffer;
    for (int i = 1; i < argc; i++) {
      if (*argv[i] == '-') {
        error_check = FlagsParser(&info, argv[i], argv[0]);
      } else {
        if (error_check) {
          OldBuffer = FileNameSave;
          FileNameSave =
              (int *)realloc(FileNameSave, (FileIndexArgc + 1) * sizeof(int));
          if (FileNameSave != NULL) {
            FileNameSave[FileIndexArgc] = i;
            FileIndexArgc++;
            info.file_count = FileIndexArgc;
          } else {
            free(OldBuffer);
            free(FileNameSave);
            exit(1);
          }
        }
      }
    }
    if (error_check) CatArgs(&info, FileNameSave, argv, FileIndexArgc);
  }

  free(FileNameSave);
  return 0;
}
