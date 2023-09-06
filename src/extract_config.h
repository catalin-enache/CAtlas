
#ifndef ATLAS_EXTRACT_CONFIG_H
#define ATLAS_EXTRACT_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definitions.h"
#include "dir_files_utils.h"

typedef struct {
    int value;
    int err;
} IntResult;

typedef struct {
    double value;
    int err;
} DoubleResult;

typedef struct {
    int i;
    double d;
    char s[MAX_LINE_LENGTH];
    char type;
} Value;


typedef struct {
    char key[MAX_LINE_LENGTH];
    Value value;
} KeyValue;

typedef struct {
    int position;
    int width;
    int height;
} FillInEntry;

void print_key_value(KeyValue kv);
KeyValue* extract_config(const char *fileName, int *num_entries);
KeyValue* config_find(KeyValue *kvArr, int length, const char *needle);
IntResult get_int(const char *str);
FillInEntry* extract_fill_in_array(const char* fill_in_value, int* fill_in_array_length);
FillInEntry* find_fill_in_entry(FillInEntry* fill_in_array, int fill_in_array_length, int position);
int compareFillInEntry(const void* a, const void* b);

#endif //ATLAS_EXTRACT_CONFIG_H