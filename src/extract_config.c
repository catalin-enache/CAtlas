#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extract_config.h"
#include "dir_files_utils.h"

void print_key_value(KeyValue kv) {
    if(kv.value.type == 'i') {
        printf("%s %d (int)\n", kv.key, kv.value.i);
    } else if(kv.value.type == 'd') {
        printf("%s %lf (double)\n", kv.key, kv.value.d);
    } else {
        printf("%s %s (string)\n", kv.key, kv.value.s);
    }
}

DoubleResult get_double(const char *str) {
    DoubleResult result = {.value = 0, .err = 1};
    char* stopped = NULL;
    double numeric = strtod(str, &stopped);
    if (!*stopped) {
        result.err = 0;
        result.value = numeric;
    }
    return result;
}

IntResult get_int(const char *str) {
    IntResult result = {.value = 0, .err = 1};
    char* stopped = NULL;
    double numeric = strtol(str, &stopped, 10);
    if (!*stopped) {
        result.err = 0;
        result.value = numeric;
    }
    return result;
}

Value get_value(const char *str) {
    IntResult intRes;
    DoubleResult doubleRes;
    Value value;
    intRes = get_int(str);
    if (!intRes.err) {
        value.i = intRes.value;
        value.type = 'i';
        return value;
    }
    doubleRes = get_double(str);
    if (!doubleRes.err) {
        value.d = doubleRes.value;
        value.type = 'd';
        return value;
    }

    strncpy(value.s, str, strlen(str) + 1); // include null char
    value.type = 's';
    return value;
}

KeyValue extract_line(const char *line) {
    char key[MAX_LINE_LENGTH];
    char val[MAX_LINE_LENGTH];
    KeyValue kv;
    sscanf(line, "%s %s", key, val);
    Value v = get_value(val);
    strncpy(kv.key, key, strlen(key) + 1);
    kv.value = v;
    return kv;
}

KeyValue* extract_config(const char *fileName, int *num_entries) {
    char** lines = read_lines(fileName, num_entries);

    if (lines == NULL) {
        printf("extract_config could not read config lines\n");
        return NULL;
    }

    KeyValue* kvArr = malloc(*num_entries * sizeof(KeyValue));
    if (kvArr == NULL) {
        printf("extract_config could not allocate kvArr\n");
        return NULL;
    }

    for (int i = 0; i < *num_entries; i++) {
        kvArr[i] = extract_line(lines[i]);
        free(lines[i]);
    }

    free(lines);
    return kvArr;
}

KeyValue*  config_find(KeyValue *kvArr, int kvArrLength, const char *needle) {
    for (int i = 0; i < kvArrLength; i++) {
        if(strcmp(kvArr[i].key, needle) == 0) {
            return &kvArr[i];
        }
    }
    return NULL;
}
