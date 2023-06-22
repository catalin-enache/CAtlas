#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dir_files_utils.h"
#include "definitions.h"

const char *extensions[] = {".png", ".apng", ".jpg", ".jpeg", ".webp", ".tif", ".tiff", ".bmp", ".gif", ".tga"};
const int extensions_length = sizeof(extensions) / sizeof(extensions[0]);

int ends_with_extension(const char *filename, const char *extension) {
    size_t len_f = strlen(filename);
    size_t len_e = strlen(extension);
    if (len_f < len_e) return 0;
    return strcmp(filename + len_f - len_e, extension) == 0;
}

int should_keep(const char *fileName) {
    int keep = 0;
    for (int i = 0; i < extensions_length; i++) {
        // if ends with an img extension and is not the atlas image, keep it
        if (ends_with_extension(fileName, extensions[i]) && strstr(fileName, "atlas.") == NULL) {
            keep = 1;
            break;
        }
    }
    return keep;
}


char** list_files(const char* dir_name, int* num_files, bool concat_path) {
    DIR *dir;
    struct dirent *entry;
    char **files;
    int count = 0;
    size_t dir_name_len = strlen(dir_name);

    dir = opendir(dir_name);
    if (dir == NULL) {
        printf("Error opening directory\n");
        return NULL;
    }

    while((entry = readdir(dir)) != NULL) {
        if(should_keep(entry->d_name)) {
            count++;
        }
    }

    files = malloc(count * sizeof(char*));
    if (files == NULL) {
        printf("Error allocating memory\n");
        return NULL;
    }

    rewinddir(dir);
    count = 0;

    while((entry = readdir(dir)) != NULL) {
        if(should_keep(entry->d_name)) {
            if (concat_path) {
                char path[FILENAME_MAX];
                strncpy(path, dir_name, dir_name_len + 1);
                strncat(path + dir_name_len, entry->d_name, strlen(entry->d_name));
                files[count] = strdup(path);
            } else {
                files[count] = strdup(entry->d_name);
            }
            count++;
        }
    }

    closedir(dir);
    *num_files = count;
    return files;
}

char** read_lines(const char* filename, int* num_lines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    // Allocate an initial block of memory for the lines
    int capacity = 10;
    char **lines = malloc(capacity * sizeof(char*));
    if (lines == NULL) {
        printf("Error allocating memory for reading lines.\n");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    *num_lines = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        // Resize the array if necessary
        if (*num_lines >= capacity) {
            capacity *= 2;
            lines = realloc(lines, capacity * sizeof(char*));
            if (lines == NULL) {
                printf("Error reallocating memory.\n");
                return NULL;
            }
        }
        line[strcspn(line, "\n")] = '\0';
        // Store the line
        lines[*num_lines] = strdup(line);
        (*num_lines)++;
    }
    fclose(file);
    return lines;
}
