#include "extract_config.h"


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

FillInEntry* extract_fill_in_array(const char* fill_in_value, int* fill_in_array_length) {
    if (strcmp(fill_in_value, "none") == 0) {
        *fill_in_array_length = 0;
        return NULL;
    }
    // int commas_num = count_char(fill_in_value, ',');
    // *fill_in_array_length = commas_num + 1;

    char **tokens = split_string(fill_in_value, ",", fill_in_array_length);
    // char tokens_info[256] = "";
    // arrayToString(tokens_info, (void *)tokens, (int[]){*fill_in_array_length}, 1, 0, "string", sizeof(char*), "%s", 64);
    // printf("tokens: %s\n", tokens_info);

    FillInEntry* fill_in_array = malloc(*fill_in_array_length * sizeof(FillInEntry));
    if (fill_in_array == NULL) { printf("extract_fill_in_array could not allocate fill_in_array\n"); return NULL; }

    for (int i = 0; i < *fill_in_array_length; i++) {
        char **tokens2 = split_string(tokens[i], ":", NULL);
        fill_in_array[i].position = get_int(tokens2[0]).value;

        char **tokens3 = split_string(tokens2[1], "x", NULL);
        fill_in_array[i].width = get_int(tokens3[0]).value;
        fill_in_array[i].height = get_int(tokens3[1]).value;

        free(tokens2[0]);
        free(tokens2[1]);
        free(tokens2);
        free(tokens3[0]);
        free(tokens3[1]);
        free(tokens3);
    }

    return fill_in_array;
}

FillInEntry* find_fill_in_entry(FillInEntry* fill_in_array, int fill_in_array_length, int position) {
    for (int i = 0; i < fill_in_array_length; i++) {
        if (fill_in_array[i].position == position) {
            return &fill_in_array[i];
        }
    }
    return NULL;
}