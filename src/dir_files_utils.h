
#ifndef ATLAS_DIR_FILES_UTILS_H
#define ATLAS_DIR_FILES_UTILS_H

#include <stdbool.h>

typedef enum {
    WRITE_LINES_WRITTEN, WRITE_LINES_FILE_EXIST, WRITE_LINES_ERROR
} WriteLinesStatus;

typedef enum {
    MAKE_DIR_DONE, MAKE_DIR_EXIST, MAKE_DIR_ERROR
} MakeDirStatus;

MakeDirStatus make_dir_if_not_exists(const char *dir_name);
bool file_exists(const char *file_name);
WriteLinesStatus write_lines(const char *file_name, const char **lines, int lines_num, bool cancel_if_file_exists);
char** list_files(const char* dir_name, int* num_files, bool concat_path);
char** read_lines(const char* file_name, int* num_lines);


#endif //ATLAS_DIR_FILES_UTILS_H
