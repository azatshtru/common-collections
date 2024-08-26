#include "include/vector.h"
#include "include/hashmap_chassis.h"
#include <stdio.h>
#include <string.h>

float string_cmp(char* a, char* b) {

}

unsigned long string_hash(char* a) {

}

__hashmap_impl__(sample, char*, int, string_cmp, string_hash);

int main() {
    printf("hello world!\n");

    vector(int) v = vec_new(int);

    vec_push(v, 7);
    vec_push(v, 23);
    vec_push(v, 3);
    vec_insert(v, 1, -3);
    vec_zap(v, -2);
    vec_print_primitive(v, "%d");

    foo();

    vec_free(v, NULL);

    return 0;
}