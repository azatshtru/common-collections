#ifndef VECTOR
#define VECTOR

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VEC_INIT_SIZE 8

typedef unsigned int uint32;
typedef unsigned char byte;

typedef struct _vector {
    byte* data;
    uint32 type_size;
    uint32 len;
    uint32 cap;
} Vector;

void** vec_allocate(uint32 len, uint32 type_size);
void** vec_from(uint32 len, ...);
void vec_free(void* vec, void(*free_fn)(byte*));
void vec_resize(Vector* vec, uint32 new_len);
void vec_zap(void* vec, int index);

#define vector(type) type **
#define vec_new(type) (type**)vec_allocate(VEC_INIT_SIZE, sizeof(type))
#define vi(vec_ptr, _index) (*(vec_ptr))[(_index)]
#define vec_get(vec_ptr, _index) (*(vec_ptr))[(_index)]
#define vec_set(vec_ptr, _index, value) (*(vec_ptr))[(_index)] = (value)

#define vec_print(vec, print_fn)                    \
do {                                                \
    printf("vec![");                                \
    for(int i = 0; i < ((Vector*)(vec))->len; i++) {\
        print_fn((vi(vec, i)));                     \
        printf(", ");                               \
    }                                               \
    printf("\b\b]\n");                              \
} while(0)

#define vec_print_primitive(vec, identifier)        \
do {                                                \
    printf("vec![");                                \
    for(int i = 0; i < ((Vector*)(vec))->len; i++) {\
        printf((identifier), (vi(vec, i)));         \
        printf(", ");                               \
    }                                               \
    printf("\b\b]\n");                              \
} while(0)

#define vec_push(vec, value)                        \
do {                                                \
    Vector* v = (Vector*)(vec);                     \
    if(v->len >= v->cap) {                          \
        vec_resize(v, v->cap * 2);                  \
    }                                               \
    (*(vec))[v->len] = value;                       \
    ++v->len;                                       \
} while(0)

#define vec_insert(vec, _index, value)              \
do {                                                \
    vec_push((vec), 0);                             \
    Vector* v = (Vector*)(vec);                     \
    memcpy(*(vec) + _index + 1,                     \
        *(vec) + _index,                            \
        v->type_size * (v->len - _index - 1));      \
    (*(vec))[_index] = (value);                     \
} while(0)

#endif