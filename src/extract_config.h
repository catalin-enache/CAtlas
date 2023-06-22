
#ifndef ATLAS_EXTRACT_CONFIG_H
#define ATLAS_EXTRACT_CONFIG_H

#include "definitions.h"

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

void print_key_value(KeyValue kv);
KeyValue* extract_config(const char *fileName, int *num_entries);
KeyValue* config_find(KeyValue *kvArr, int length, const char *needle);

#endif //ATLAS_EXTRACT_CONFIG_H