#include <string.h>
#include <ctype.h>

#include "utils.h"
// https://stackoverflow.com/questions/656542/trim-a-string-in-c
char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}
