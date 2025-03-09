#ifndef VECTOR
#define VECTOR

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VEC_INIT_SIZE 1

typedef unsigned int uint32;
typedef unsigned char byte;

typedef struct _vector {
    byte* data;
    byte* tmp;
    uint32 type_size;
    uint32 len;
    uint32 cap;
} Vector;

void* vec_allocate(uint32 cap, uint32 type_size);
void* vec_from(uint32 len, ...);
void vec_free(void* vec, void(*free_fn)(void*));
void vec_resize(Vector* vec, uint32 new_len);
int vec_zap(void* vec, int index, void(*free_fn)(void*));
int vec_cmp(void* v1_ptr, void* v2_ptr);
uint32 vec_len(void* vec);
int vec_shush_index(void* vec, void* value_ptr);
void vec_clear(void* vec);
void vec_reverse(void* vec);

#define vector(type) type **
#define vec_new(type) (type**)vec_allocate(VEC_INIT_SIZE, sizeof(type))
#define vi(vec_ptr, _index) (*(vec_ptr))[(_index)]
#define vec_as_array(vec_ptr) (*(vec_ptr))
#define vec_get(vec_ptr, _index) (*(vec_ptr))[(_index)]
#define vec_set(vec_ptr, _index, value) (*(vec_ptr))[(_index)] = (value)

#define vec_index(vec, value) (**((vec)+1)=value, vec_shush_index(vec, *((vec)+1)))
#define vec_contains(vec, value) (**((vec)+1)=value, -1 != vec_shush_index(vec, *((vec)+1)))

#define vec_pop(vec, index) (**((vec)+1)=vec_get((vec), (index >= 0) ? (index) : (((Vector*)(vec))->len + (index))), vec_zap((vec), (index), NULL), **((vec)+1))

#define vec_print(vec, print_fn)                    \
do {                                                \
    printf("vec![");                                \
    for(int i = 0; i < ((Vector*)(vec))->len; i++) {\
        print_fn((vec_get(vec, i)));                \
        printf(", ");                               \
    }                                               \
    printf("\b\b]\n");                              \
} while(0)

#define vec_print_primitive(vec, identifier)        \
do {                                                \
    printf("vec![");                                \
    for(int i = 0; i < ((Vector*)(vec))->len; i++) {\
        printf((identifier), (vec_get(vec, i)));    \
        printf(", ");                               \
    }                                               \
    printf("\b\b]\n");                              \
} while(0)

#define vec_push(vec, value)                   \
    do                                         \
    {                                          \
        Vector *__v__ = (Vector *)(vec);       \
        if (__v__->len >= __v__->cap)          \
        {                                      \
            vec_resize(__v__, __v__->cap * 2); \
        }                                      \
        (*(vec))[__v__->len] = value;          \
        ++__v__->len;                          \
    } while (0)

#define vec_insert(vec, _index, value)                        \
    do                                                        \
    {                                                         \
        Vector *__v__ = (Vector *)(vec);                      \
        if(_index > __v__->len) { break; }                    \
        **((vec)+1) = value;                                  \
        vec_push((vec), **((vec)+1));                         \
        memmove(*(vec) + _index + 1,                          \
                *(vec) + _index,                              \
                __v__->type_size * (__v__->len - _index - 1)    \
        );                                                      \
        (*(vec))[_index] = **((vec)+1);                         \
    } while (0)

#endif

