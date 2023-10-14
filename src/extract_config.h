
#ifndef ATLAS_EXTRACT_CONFIG_H
#define ATLAS_EXTRACT_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definitions.h"
#include "dir_files_utils.h"
#include "shared_types.h"
#include "calculations.h"

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
FillInEntry* extract_fill_in_array(const char* fill_in_string, int* fill_in_array_length);
FillInEntry* find_fill_in_entry(FillInEntry* fill_in_array, int fill_in_array_length, int position);
int compareFillInEntry(const void* a, const void* b);
AbsolutePositioningEntry* extract_absolute_positioning_array(const char* absolute_positioning_string, int* absolute_positioning_array_length);
AbsolutePositioningEntry* find_absolute_positioning_entry_by_name(AbsolutePositioningEntry* absolute_positioning_array, int absolute_positioning_array_length, char * name);
int get_absolute_position_canvas_width(AbsolutePositioningEntry* absolute_positioning_array, int absolute_positioning_array_length);
int get_absolute_position_canvas_height(AbsolutePositioningEntry* absolute_positioning_array, int absolute_positioning_array_length);
#endif //ATLAS_EXTRACT_CONFIG_H