#include "include/vector.h"
#include <stdio.h>

int main() {
    printf("hello world!\n");

    vector(int) au = vec_new(int);

    vec_push(au, 7);
    vec_push(au, 23);
    vec_push(au, 3);
    vec_insert(au, 1, -3);
    vec_zap(au, -2);
    vec_print_primitive(au, "%d");

    vec_free(au, NULL);
    return 0;
}