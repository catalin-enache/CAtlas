//
// Created by catal on 19-Jun-23.
//

#ifndef ATLAS_DIR_FILES_UTILS_H
#define ATLAS_DIR_FILES_UTILS_H
char** list_files(const char* dir_name, int* num_files, int concat_path);
char** read_lines(const char* filename, int* num_lines);
#endif //ATLAS_DIR_FILES_UTILS_H
