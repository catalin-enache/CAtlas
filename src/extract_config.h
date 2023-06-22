
#ifndef ATLAS_EXTRACT_CONFIG_H
#define ATLAS_EXTRACT_CONFIG_H

#define MAX_LINE_LENGTH 1024

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

KeyValue* extract_config(const char *fileName, int *num_entries, int *err);
void printKeyValue(KeyValue kv);
int config_find(KeyValue *kvArr, int length, const char *needle, KeyValue **kvOut);

#endif //ATLAS_EXTRACT_CONFIG_H