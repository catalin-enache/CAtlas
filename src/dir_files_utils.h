
#ifndef ATLAS_DIR_FILES_UTILS_H
#define ATLAS_DIR_FILES_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "definitions.h"
#include "utils.h"

typedef bool (*AllowString)(const char *);

typedef enum {
    WRITE_LINES_WRITTEN, WRITE_LINES_FILE_EXIST, WRITE_LINES_ERROR
} WriteLinesStatus;

typedef enum {
    MAKE_DIR_DONE, MAKE_DIR_EXIST, MAKE_DIR_ERROR
} MakeDirStatus;

bool ends_with_extension(const char *filename, const char *extension);
bool is_image_file(const char *file_name);
bool is_image_file_and_not_atlas(const char *file_name);
bool is_image_file_and_is_mergeable_target(const char *file_name);
bool is_image_file_and_not_extracted(const char *file_name);
bool is_image_file_and_is_mergeable_source(const char *file_name);
bool no_filter(const char *file_name);
bool file_exists(const char *file_name);
MakeDirStatus make_dir_if_not_exists(const char *dir_name);
WriteLinesStatus write_lines(const char *file_name, const char **lines, int lines_num, bool cancel_if_file_exists);
char** list_files_with_filter(const char* dir_name, int* num_files, bool concat_path, AllowString allow_string);
char** list_files(const char* dir_name, int* num_files, bool concat_path);
char** read_lines(const char* file_name, int* num_lines);


#endif //ATLAS_DIR_FILES_UTILS_H
