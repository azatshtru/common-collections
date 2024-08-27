#include "include/vector.h"
#include "include/hashset.h"
#include <stdio.h>
#include <string.h>

unsigned int int_cmp(void* a, void* b) {
    int x = *(int*)a;
    int y = *(int*)b;
    return x == y;
}

unsigned int int_hash(void* a) {
    int x = *(int*)a;
    return (long)x;
}

int main() {
    printf("hello world!\n");

    int** h = hashset_allocate(sizeof(int), int_hash, int_cmp);
    hashset_put(h, 5);
    hashset_put(h, 7);
    hashset_put(h, 566);
    hashset_put(h, 67);
    hashset_print_primitive(h, "%d");

    hashset_free(h, NULL);

    return 0;
}