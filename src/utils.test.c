#include <stdio.h>
#include "./utils.h"

char**** getHeapString3DArray() {
    char**** heapString3DArr = (char****)malloc(2 * sizeof(char***));
    heapString3DArr[0] = (char***)malloc(2 * sizeof(char**));
    *(heapString3DArr + 1) = (char***)malloc(2 * sizeof(int**)); // heapString3DArr[1]
    **heapString3DArr = (char**)malloc(2 * sizeof(double*)); // heapString3DArr[0][0]
    *(*heapString3DArr + 1) = (char**)malloc(2 * sizeof(void*)); // heapString3DArr[0][1]
    heapString3DArr[1][0] = (char**)malloc(2 * sizeof(char*));
    heapString3DArr[1][1] = (char**)malloc(2 * sizeof(char*));
    heapString3DArr[0][0][0] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[0][0][1] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[0][1][0] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[0][1][1] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[1][0][0] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[1][0][1] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[1][1][0] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[1][1][1] = (char*)malloc(64 * sizeof(char));
    heapString3DArr[0][0][0] = strcpy(heapString3DArr[0][0][0], "aaaa");
    heapString3DArr[0][0][1] = strcpy(heapString3DArr[0][0][1], "bbbb");
    heapString3DArr[0][1][0] = strcpy(heapString3DArr[0][1][0], "cccc");
    heapString3DArr[0][1][1] = strcpy(heapString3DArr[0][1][1], "dddd");
    heapString3DArr[1][0][0] = strcpy(heapString3DArr[1][0][0], "eeee");
    heapString3DArr[1][0][1] = strcpy(heapString3DArr[1][0][1], "ffff");
    heapString3DArr[1][1][0] = strcpy(heapString3DArr[1][1][0], "gggg");
    heapString3DArr[1][1][1] = strcpy(heapString3DArr[1][1][1], "hhhh");

    return heapString3DArr;
}

char *** getHeapString2DArray() {
    char*** heapString2DArr = (char***)malloc(2 * sizeof(char**));
    heapString2DArr[0] = (char**)malloc(2 * sizeof(char*));
    *(heapString2DArr + 1) = (char**)malloc(2 * sizeof(char*)); // heapString2DArr[1]
    heapString2DArr[0][0] = (char*)malloc(64 * sizeof(char));
    heapString2DArr[0][1] = (char*)malloc(64 * sizeof(char));
    heapString2DArr[1][0] = (char*)malloc(64 * sizeof(char));
    heapString2DArr[1][1] = (char*)malloc(64 * sizeof(char));
    heapString2DArr[0][0] = strcpy(heapString2DArr[0][0], "aaaa");
    heapString2DArr[0][1] = strcpy(heapString2DArr[0][1], "bbbb");
    heapString2DArr[1][0] = strcpy(heapString2DArr[1][0], "cccc");
    heapString2DArr[1][1] = strcpy(heapString2DArr[1][1], "dddd");

    return heapString2DArr;
}

char ** getHeapString1DArray() {
    char** heapString1DArr = (char**)malloc(2 * sizeof(char*));
    heapString1DArr[0] = (char*)malloc(64 * sizeof(char));
    heapString1DArr[1] = (char*)malloc(64 * sizeof(char));
    heapString1DArr[0] = strcpy(heapString1DArr[0], "aaaa");
    heapString1DArr[1] = strcpy(heapString1DArr[1], "bbbb");

    return heapString1DArr;
}

int*** getHeapInt3DArray() {
    int*** heapInt3DArr = (int***)malloc(2 * sizeof(int**));
    heapInt3DArr[0] = (int**)malloc(2 * sizeof(int*));
    *(heapInt3DArr + 1) = (int**)malloc(2 * sizeof(int*)); // heapInt3DArr[1]
    **heapInt3DArr = (int*)malloc(2 * sizeof(int)); // heapInt3DArr[0][0]
    *(*heapInt3DArr + 1) = (int*)malloc(2 * sizeof(int)); // heapInt3DArr[0][1]
    heapInt3DArr[1][0] = (int*)malloc(2 * sizeof(int));
    heapInt3DArr[1][1] = (int*)malloc(2 * sizeof(int));
    heapInt3DArr[0][0][0] = 1;
    heapInt3DArr[0][0][1] = 2;
    heapInt3DArr[0][1][0] = 3;
    heapInt3DArr[0][1][1] = 4;
    heapInt3DArr[1][0][0] = 5;
    heapInt3DArr[1][0][1] = 6;
    heapInt3DArr[1][1][0] = 7;
    heapInt3DArr[1][1][1] = 8;

    return heapInt3DArr;
}

float*** getHeapFloat3DArray() {
    float*** heapFloat3DArr = (float***)malloc(2 * sizeof(float**));
    heapFloat3DArr[0] = (float**)malloc(2 * sizeof(float*));
    *(heapFloat3DArr + 1) = (float**)malloc(2 * sizeof(float*)); // heapFloat3DArr[1]
    **heapFloat3DArr = (float*)malloc(2 * sizeof(float)); // heapFloat3DArr[0][0]
    *(*heapFloat3DArr + 1) = (float*)malloc(2 * sizeof(float)); // heapFloat3DArr[0][1]
    heapFloat3DArr[1][0] = (float*)malloc(2 * sizeof(float));
    heapFloat3DArr[1][1] = (float*)malloc(2 * sizeof(float));
    heapFloat3DArr[0][0][0] = 1.1;
    heapFloat3DArr[0][0][1] = 2.2;
    heapFloat3DArr[0][1][0] = 3.3;
    heapFloat3DArr[0][1][1] = 4.4;
    heapFloat3DArr[1][0][0] = 5.5;
    heapFloat3DArr[1][0][1] = 6.6;
    heapFloat3DArr[1][1][0] = 7.7;
    heapFloat3DArr[1][1][1] = 8.8;

    return heapFloat3DArr;
}

char*** getHeapChar3DArray() {
    char*** heapChar3DArr = (char***)malloc(2 * sizeof(char**));
    heapChar3DArr[0] = (char**)malloc(2 * sizeof(char*));
    *(heapChar3DArr + 1) = (char**)malloc(2 * sizeof(char*)); // heapChar3DArr[1]
    **heapChar3DArr = (char*)malloc(2 * sizeof(char)); // heapChar3DArr[0][0]
    *(*heapChar3DArr + 1) = (char*)malloc(2 * sizeof(char)); // heapChar3DArr[0][1]
    heapChar3DArr[1][0] = (char*)malloc(2 * sizeof(char));
    heapChar3DArr[1][1] = (char*)malloc(2 * sizeof(char));
    heapChar3DArr[0][0][0] = 'a';
    heapChar3DArr[0][0][1] = 'b';
    heapChar3DArr[0][1][0] = 'c';
    heapChar3DArr[0][1][1] = 'd';
    heapChar3DArr[1][0][0] = 'e';
    heapChar3DArr[1][0][1] = 'f';
    heapChar3DArr[1][1][0] = 'g';
    heapChar3DArr[1][1][1] = 'h';

    return heapChar3DArr;
}

void test_arrayToString() {
    char**** heapString3DArr = getHeapString3DArray();
    int heapString3DArrDimensions[] = {2,2,2};
    char heapString3DArrAsString[1024] = "";
    arrayToString(heapString3DArrAsString, (void*)heapString3DArr, heapString3DArrDimensions, 3, 0, "string", sizeof(char*), "%s", 32);
    if (strcmp(heapString3DArrAsString, "[[[aaaa, bbbb], [cccc, dddd]], [[eeee, ffff], [gggg, hhhh]]]") == 0) {
        printf("heapString3DArr(success) = %s\n", heapString3DArrAsString);
    } else {
        printf("heapString3DArr(failure) = %s\n", heapString3DArrAsString);
    }

    char stackString3dArr[][2][2][64] = {{{"AAAA", "BBBB"}, {"CCCC", "DDDD"}}, {{"EEEE", "FFFF"}, {"GGGG", "HHHH"}}};
    int stackString3dArrDimensions[] = {2,2,2};
    char stackString3dArrAsString[1024] = "";
    arrayToString(stackString3dArrAsString, (void*)stackString3dArr, stackString3dArrDimensions, 3, 1, "string", 64, "%s", 32);
    if (strcmp(stackString3dArrAsString, "[[[AAAA, BBBB], [CCCC, DDDD]], [[EEEE, FFFF], [GGGG, HHHH]]]") == 0) {
        printf("stackString3dArr(success) = %s\n", stackString3dArrAsString);
    } else {
        printf("stackString3dArr(failure) = %s\n", stackString3dArrAsString);
    }

    int*** heapInt3DArr = getHeapInt3DArray();
    int heapInt3DArrDimensions[] = {2,2,2};
    char heapInt3DArrAsString[1024] = "";
    arrayToString(heapInt3DArrAsString, (void*)heapInt3DArr, heapInt3DArrDimensions, 3, 0, "int", sizeof(int), "%d", 32);
    if (strcmp(heapInt3DArrAsString, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]") == 0) {
        printf("heapInt3DArr(success) = %s\n", heapInt3DArrAsString);
    } else {
        printf("heapInt3DArr(failure) = %s\n", heapInt3DArrAsString);
    }

    int stackInt3dArr[][2][2] = {{{11, 12}, {13, 14}}, {{15, 16}, {17, 18}}};
    int stackInt3dArrDimensions[] = {2,2,2};
    char stackInt3dArrAsString[1024] = "";
    arrayToString(stackInt3dArrAsString, (void*)stackInt3dArr, stackInt3dArrDimensions, 3, 1, "int", sizeof(int), "%d", 32);
    if (strcmp(stackInt3dArrAsString, "[[[11, 12], [13, 14]], [[15, 16], [17, 18]]]") == 0) {
        printf("stackInt3dArr(success) = %s\n", stackInt3dArrAsString);
    } else {
        printf("stackInt3dArr(failure) = %s\n", stackInt3dArrAsString);
    }

    float*** heapFloat3DArr = getHeapFloat3DArray();
    int heapFloat3DArrDimensions[] = {2,2,2};
    char heapFloat3DArrAsString[1024] = "";
    arrayToString(heapFloat3DArrAsString, (void*)heapFloat3DArr, heapFloat3DArrDimensions, 3, 0, "float", sizeof(float), "%.2f", 32);
    if (strcmp(heapFloat3DArrAsString, "[[[1.10, 2.20], [3.30, 4.40]], [[5.50, 6.60], [7.70, 8.80]]]") == 0) {
        printf("heapFloat3DArr(success) = %s\n", heapFloat3DArrAsString);
    } else {
        printf("heapFloat3DArr(failure) = %s\n", heapFloat3DArrAsString);
    }

    float stackFloat3dArr[][2][2] = {{{11.11, 11.12}, {11.13, 11.14}}, {{11.15, 11.16}, {11.17, 11.18}}};
    int stackFloat3dArrDimensions[] = {2,2,2};
    char stackFloat3dArrAsString[1024] = "";
    arrayToString(stackFloat3dArrAsString, (void*)stackFloat3dArr, stackFloat3dArrDimensions, 3, 1, "float", sizeof(float), "%.2f", 32);
    if (strcmp(stackFloat3dArrAsString, "[[[11.11, 11.12], [11.13, 11.14]], [[11.15, 11.16], [11.17, 11.18]]]") == 0) {
        printf("stackFloat3dArr(success) = %s\n", stackFloat3dArrAsString);
    } else {
        printf("stackFloat3dArr(failure) = %s\n", stackFloat3dArrAsString);
    }

    char*** heapChar3DArr = getHeapChar3DArray();
    int heapChar3DArrDimensions[] = {2,2,2};
    char heapChar3DArrAsString[1024] = "";
    arrayToString(heapChar3DArrAsString, (void*)heapChar3DArr, heapChar3DArrDimensions, 3, 0, "char", sizeof(char), "%c", 32);
    if (strcmp(heapChar3DArrAsString, "[[[a, b], [c, d]], [[e, f], [g, h]]]") == 0) {
        printf("heapChar3DArr(success) = %s\n", heapChar3DArrAsString);
    } else {
        printf("heapChar3DArr(failure) = %s\n", heapChar3DArrAsString);
    }

    char stackChar3dArr[][2][2] = {{{'A', 'B'}, {'C', 'D'}}, {{'E', 'F'}, {'G', 'H'}}};
    int stackChar3dArrDimensions[] = {2,2,2};
    char stackChar3dArrAsString[1024] = "";
    arrayToString(stackChar3dArrAsString, (void*)stackChar3dArr, stackChar3dArrDimensions, 3, 1, "char", sizeof(char), "%c", 32);
    if (strcmp(stackChar3dArrAsString, "[[[A, B], [C, D]], [[E, F], [G, H]]]") == 0) {
        printf("stackChar3dArr(success) = %s\n", stackChar3dArrAsString);
    } else {
        printf("stackChar3dArr(failure) = %s\n", stackChar3dArrAsString);
    }

    char*** heapString2DArr = getHeapString2DArray();
    int heapString2DArrDimensions[] = {2,2};
    char heapString2DArrAsString[1024] = "";
    arrayToString(heapString2DArrAsString, (void*)heapString2DArr, heapString2DArrDimensions, 2, 0, "string", sizeof(char*), "%s", 32);
    if (strcmp(heapString2DArrAsString, "[[aaaa, bbbb], [cccc, dddd]]") == 0) {
        printf("heapString2DArr(success) = %s\n", heapString2DArrAsString);
    } else {
        printf("heapString2DArr(failure) = %s\n", heapString2DArrAsString);
    }

    char stackString2dArr[][2][64] = {{"AAAA", "BBBB"}, {"CCCC", "DDDD"}};
    int stackString2dArrDimensions[] = {2,2};
    char stackString2dArrAsString[1024] = "";
    arrayToString(stackString2dArrAsString, (void*)stackString2dArr, stackString2dArrDimensions, 2, 1, "string", 64, "%s", 32);
    if (strcmp(stackString2dArrAsString, "[[AAAA, BBBB], [CCCC, DDDD]]") == 0) {
        printf("stackString2dArr(success) = %s\n", stackString2dArrAsString);
    } else {
        printf("stackString2dArr(failure) = %s\n", stackString2dArrAsString);
    }

    char** heapString1DArr = getHeapString1DArray();
    int heapString1DArrDimensions[] = {2};
    char heapString1DArrAsString[1024] = "";
    arrayToString(heapString1DArrAsString, (void*)heapString1DArr, heapString1DArrDimensions, 1, 0, "string", sizeof(char*), "%s", 32);
    if (strcmp(heapString1DArrAsString, "[aaaa, bbbb]") == 0) {
        printf("heapString1DArr(success) = %s\n", heapString1DArrAsString);
    } else {
        printf("heapString1DArr(failure) = %s\n", heapString1DArrAsString);
    }

    char stackString1dArr[][64] = {"AAAA", "BBBB"};
    int stackString1dArrDimensions[] = {2};
    char stackString1dArrAsString[1024] = "";
    arrayToString(stackString1dArrAsString, (void*)stackString1dArr, stackString1dArrDimensions, 1, 1, "string", 64, "%s", 32);
    if (strcmp(stackString1dArrAsString, "[AAAA, BBBB]") == 0) {
        printf("stackString1dArr(success) = %s\n", stackString1dArrAsString);
    } else {
        printf("stackString1dArr(failure) = %s\n", stackString1dArrAsString);
    }
}

void test_print_3D_array() {
    char**** heapString3DArr = getHeapString3DArray();
    char *heapString3DArrAsString = print_3D_array((void*)heapString3DArr, sizeof(char*), "string", 0, "%s", 2,2,2);
    if (strcmp(heapString3DArrAsString, "[[[aaaa, bbbb], [cccc, dddd]], [[eeee, ffff], [gggg, hhhh]]]") == 0) {
        printf("print_3D_array(success) = %s\n", heapString3DArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", heapString3DArrAsString);
    }

    char stackString3dArr[][2][2][64] = {{{"AAAA", "BBBB"}, {"CCCC", "DDDD"}}, {{"EEEE", "FFFF"}, {"GGGG", "HHHH"}}};
    char *stackString3dArrAsString = print_3D_array((void*)stackString3dArr, 64, "string", 1, "%s", 2,2,2);
    if (strcmp(stackString3dArrAsString, "[[[AAAA, BBBB], [CCCC, DDDD]], [[EEEE, FFFF], [GGGG, HHHH]]]") == 0) {
        printf("print_3D_array(success) = %s\n", stackString3dArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", stackString3dArrAsString);
    }

    int*** heapInt3DArr = getHeapInt3DArray();
    char *heapInt3DArrAsString = print_3D_array((void*)heapInt3DArr, sizeof(int), "int", 0, "%d", 2,2,2);
    if (strcmp(heapInt3DArrAsString, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]") == 0) {
        printf("print_3D_array(success) = %s\n", heapInt3DArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", heapInt3DArrAsString);
    }

    int stackInt3dArr[][2][2] = {{{11, 12}, {13, 14}}, {{15, 16}, {17, 18}}};
    char *stackInt3dArrAsString = print_3D_array((void*)stackInt3dArr, sizeof(int), "int", 1, "%d", 2,2,2);
    if (strcmp(stackInt3dArrAsString, "[[[11, 12], [13, 14]], [[15, 16], [17, 18]]]") == 0) {
        printf("print_3D_array(success) = %s\n", stackInt3dArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", stackInt3dArrAsString);
    }

    float*** heapFloat3DArr = getHeapFloat3DArray();
    char *heapFloat3DArrAsString = print_3D_array((void*)heapFloat3DArr, sizeof(float), "float", 0, "%.2f", 2,2,2);
    if (strcmp(heapFloat3DArrAsString, "[[[1.10, 2.20], [3.30, 4.40]], [[5.50, 6.60], [7.70, 8.80]]]") == 0) {
        printf("print_3D_array(success) = %s\n", heapFloat3DArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", heapFloat3DArrAsString);
    }

    float stackFloat3dArr[][2][2] = {{{11.11, 11.12}, {11.13, 11.14}}, {{11.15, 11.16}, {11.17, 11.18}}};
    char *stackFloat3dArrAsString = print_3D_array((void*)stackFloat3dArr, sizeof(float), "float", 1, "%.2f", 2,2,2);
    if (strcmp(stackFloat3dArrAsString, "[[[11.11, 11.12], [11.13, 11.14]], [[11.15, 11.16], [11.17, 11.18]]]") == 0) {
        printf("print_3D_array(success) = %s\n", stackFloat3dArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", stackFloat3dArrAsString);
    }

    char*** heapChar3DArr = getHeapChar3DArray();
    char *heapChar3DArrAsString = print_3D_array((void*)heapChar3DArr, sizeof(char), "char", 0, "%c", 2,2,2);
    if (strcmp(heapChar3DArrAsString, "[[[a, b], [c, d]], [[e, f], [g, h]]]") == 0) {
        printf("print_3D_array(success) = %s\n", heapChar3DArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", heapChar3DArrAsString);
    }

    char stackChar3dArr[][2][2] = {{{'A', 'B'}, {'C', 'D'}}, {{'E', 'F'}, {'G', 'H'}}};
    char *stackChar3dArrAsString = print_3D_array((void*)stackChar3dArr, sizeof(char), "char", 1, "%c", 2,2,2);
    if (strcmp(stackChar3dArrAsString, "[[[A, B], [C, D]], [[E, F], [G, H]]]") == 0) {
        printf("print_3D_array(success) = %s\n", stackChar3dArrAsString);
    } else {
        printf("print_3D_array(failure) = %s\n", stackChar3dArrAsString);
    }
}

void test_print_3d_array_of_define_template() {
    int_2_2_2 heapInt3DArr = getHeapInt3DArray();
    char *heapInt3dArrAsString = print_3d_array_of_int_2_2_2(heapInt3DArr, 2, 2, 2);
    if (strcmp(heapInt3dArrAsString, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]") == 0) {
        printf("print_3d_array_of_int_2_2_2(success) = %s\n", heapInt3dArrAsString);
    } else {
        printf("print_3d_array_of_int_2_2_2(failure) = %s\n", heapInt3dArrAsString);
    }

    int_2x2x2 stackInt3dArr = {{{11, 12}, {13, 14}}, {{15, 16}, {17, 18}}};
    char *stackInt3dArrAsString = print_3d_array_of_int_2x2x2(stackInt3dArr, 2, 2, 2);
    if (strcmp(stackInt3dArrAsString, "[[[11, 12], [13, 14]], [[15, 16], [17, 18]]]") == 0) {
        printf("print_3d_array_of_int_2x2x2(success) = %s\n", stackInt3dArrAsString);
    } else {
        printf("print_3d_array_of_int_2x2x2(failure) = %s\n", stackInt3dArrAsString);
    }
}

void test_array_insert() {
    size_t int_arr_size = 5;
    int *heap_int_arr = malloc(int_arr_size * sizeof(int));
    for(int i = 0; i < int_arr_size; i++) {
        heap_int_arr[i] = i + 1;
    }

    int insertIntAt0 = 9;
    int *int_res = array_insert(heap_int_arr, &insertIntAt0, 0, &int_arr_size, sizeof(int));
    int insertIntAt6 = 11;
    int_res = array_insert(int_res, &insertIntAt6, 6, &int_arr_size, sizeof(int));
    int insertIntAt3 = 8;
    int_res = array_insert(int_res, &insertIntAt3, 3, &int_arr_size, sizeof(int));

    char heapInt1dArrAsString[1024] = "";
    arrayToString(heapInt1dArrAsString, (void*)int_res, (int[]){int_arr_size}, 1, 0, "int", sizeof(int), "%d", 8);
    if (strcmp(heapInt1dArrAsString, "[9, 1, 2, 8, 3, 4, 5, 11]") == 0) {
        printf("array_insert(success) = %s\n", heapInt1dArrAsString);
    } else {
        printf("array_insert(failure) = %s\n", heapInt1dArrAsString);
    }

    size_t str_arr_size = 3;
    char **str_int_arr = (char**)malloc(int_arr_size * sizeof(char*));
    str_int_arr[0] = strdup("aa");
    str_int_arr[1] = strdup("bb");
    str_int_arr[2] = strdup("cc");

    char *insertStrAt0 = strdup("dd");
    char **str_res = array_insert(str_int_arr, &insertStrAt0, 0, &str_arr_size, sizeof(char(*)));
    char *insertStrAt3 = strdup("ee");
    str_res = array_insert(str_res, &insertStrAt3, 3, &str_arr_size, sizeof(char(*)));
    char *insertStrAt5 = strdup("ff");
    str_res = array_insert(str_res, &insertStrAt5, 5, &str_arr_size, sizeof(char(*)));

    char heapStr1dArrAsString[1024] = "";
    arrayToString(heapStr1dArrAsString, (void*)str_res, (int[]){str_arr_size}, 1, 0, "string", sizeof(char(*)), "%s", 8);
    if (strcmp(heapStr1dArrAsString, "[dd, aa, bb, ee, cc, ff]") == 0) {
        printf("array_insert(success) = %s\n", heapStr1dArrAsString);
    } else {
        printf("array_insert(failure) = %s\n", heapStr1dArrAsString);
    }
}

int main() {
    test_arrayToString();
    test_print_3D_array();
    test_print_3d_array_of_define_template();
    test_array_insert();
    return 0;
}
