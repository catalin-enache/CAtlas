
#include "errors.h"

// https://stackoverflow.com/questions/205529/passing-variable-number-of-arguments-around

//void my_printf(const char *format, ...) {
//    va_list args;
//    va_start(args, format);
//    vprintf(format, args);
//    va_end(args);
//}

int exit_with_error(const char* format, ...) {
    vips_shutdown();
    printf("\n\n\n");
    printf("====================== ERROR ===========================\n");

    // variadic args flow >>
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    // << variadic args flow

    printf("Type something then press enter ...\n");
    getchar();
    return -1;
}
