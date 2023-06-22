
#ifndef ATLAS_DIR_FILES_UTILS_H
#define ATLAS_DIR_FILES_UTILS_H

#include <stdbool.h>

bool file_exists(const char *file_name);
bool write_lines(const char *file_name, const char **lines, int lines_num, bool cancel_if_file_exists);
char** list_files(const char* dir_name, int* num_files, bool concat_path);
char** read_lines(const char* file_name, int* num_lines);

#endif //ATLAS_DIR_FILES_UTILS_H
