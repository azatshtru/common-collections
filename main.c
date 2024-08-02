#include "include/vector.h"
#include <stdio.h>

int main() {
    printf("hello world!\n");

    vector(int) v = vec_new(int);

    vec_push(v, 7);
    vec_push(v, 23);
    vec_push(v, 3);
    vec_insert(v, 1, -3);
    vec_zap(v, -2);
    vec_print_primitive(v, "%d");

    vec_free(v, NULL);
    return 0;
}