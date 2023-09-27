
#include "dir_files_utils.h"


const char *extensions[] = {".png", ".apng", ".jpg", ".jpeg", ".webp", ".tif", ".tiff", ".bmp", ".gif", ".tga", ".exr"};
const int extensions_length = sizeof(extensions) / sizeof(extensions[0]);

const char *extracted_tokens[] = {".0.", ".1.", ".2.", ".3.", ".4.", ".5.", ".6.", ".7.", ".8.", ".9."};
int extracted_tokens_size = sizeof(extracted_tokens) / sizeof(extracted_tokens[0]);

const char *mergeable_tokens[] = {"0.", "1.", "2.", "3."};
int mergeable_tokens_size = sizeof(mergeable_tokens) / sizeof(mergeable_tokens[0]);

unsigned char *get_file(const char *filename, size_t *length) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(*length);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *length, file);
    fclose(file);

    return buffer;
}


bool ends_with_extension(const char *filename, const char *extension) {
    size_t len_f = strlen(filename);
    size_t len_e = strlen(extension);
    if (len_f < len_e) return 0;
    return strcmp(filename + len_f - len_e, extension) == 0;
}

bool is_image_file(const char *file_name) {
    bool keep = false;
    for (int i = 0; i < extensions_length; i++) {
        if (ends_with_extension(file_name, extensions[i])) {
            keep = true;
            break;
        }
    }
    return keep;
}

bool is_image_file_and_not_atlas(const char *file_name) {
    return is_image_file(file_name) && (strstr(file_name, "atlas.") == NULL);
}

bool is_image_file_and_is_mergeable_target(const char *file_name) {
    return is_image_file(file_name) && (strncmp(file_name, "target.", 7) == 0);
}

bool is_image_file_and_not_extracted(const char *file_name) {
    if (!is_image_file(file_name)) return false;
    for (int i = 0; i < extracted_tokens_size; i++) {
        if (strstr(file_name, extracted_tokens[i]) != NULL) return false;
    }
    return true;
}

bool is_image_file_and_is_mergeable_source(const char *file_name) {
    if (!is_image_file(file_name)) return false;
    for (int i = 0; i < mergeable_tokens_size; i++) {
        if (strncmp(file_name, mergeable_tokens[i], 2) == 0) return true;
    }
    return false;
}

bool no_filter(const char *file_name) {
    return true;
}

bool file_exists(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

// https://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c
MakeDirStatus make_dir_if_not_exists(const char *dir_name) {
    struct stat st;
    if (stat(dir_name, &st) == -1) {
        // Directory does not exist, create it
        if (mkdir(dir_name) == -1) {
            printf("Failed to create the directory.\n");
            return MAKE_DIR_ERROR;
        }
        printf("Directory %s created.\n", dir_name);
        return MAKE_DIR_DONE;
    } else if (S_ISDIR(st.st_mode)) {
        printf("Directory %s already exists.\n", dir_name);
        return MAKE_DIR_EXIST;
    } else {
        printf("A file with the same name already exists.\n");
        return MAKE_DIR_EXIST;
    }
}

WriteLinesStatus write_lines(const char *file_name, const char **lines, int lines_num, bool cancel_if_file_exists) {
    if(cancel_if_file_exists && file_exists(file_name)) {
        printf("File already exists: %s.\n", file_name);
        return WRITE_LINES_FILE_EXIST;
    }
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Could not open file for writing %s.\n", file_name);
        return WRITE_LINES_ERROR;
    }
    for (int i = 0; i < lines_num; i++) {
        const char *line = lines[i];
        fputs(line, file);
    }
    fclose(file);
    printf("File written: %s.\n", file_name);
    return WRITE_LINES_WRITTEN;
}



char** list_files_with_filter(const char* dir_name, int* num_files, bool concat_path, AllowString allow_string) {
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
        if (!allow_string(entry->d_name)) continue;
        count++;
    }

    files = (char **)malloc(count * sizeof(char*));
    if (files == NULL) {
        printf("Error allocating memory\n");
        return NULL;
    }

    rewinddir(dir);
    count = 0;

    while((entry = readdir(dir)) != NULL) {
        if (!allow_string(entry->d_name)) continue;
        if (concat_path) {
            char path[FILENAME_MAX];
            strncpy(path, dir_name, dir_name_len + 1);
            strncat(path + dir_name_len, entry->d_name, FILENAME_MAX - dir_name_len + 1);
            files[count] = strdup(path);
        } else {
            files[count] = strdup(entry->d_name);
        }
        count++;
    }


    closedir(dir);
    *num_files = count;
    return files;
}

char** list_files(const char* dir_name, int* num_files, bool concat_path) {
    return list_files_with_filter(dir_name, num_files, concat_path, no_filter);
}

char** read_lines(const char* filename, int* num_lines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        return NULL;
    }

    // Allocate an initial block of memory for the lines
    int capacity = 10;
    char **lines = (char **)malloc(capacity * sizeof(char*));
    if (lines == NULL) {
        printf("Error allocating memory for reading lines.\n");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    *num_lines = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        strcpy(line,trim(line));
        if(!line[0] || strstr(line, "##") != NULL) continue; // ## is for comment lines, skipping them
        // Resize the array if necessary
        if (*num_lines >= capacity) {
            capacity *= 2;
            lines = (char **)realloc(lines, capacity * sizeof(char*));
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
