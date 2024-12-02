#include "../include/vector.h"

void** vec_allocate(uint32 cap, uint32 type_size) {
    byte* data = malloc(cap * type_size * sizeof(byte));
    Vector* vec_ptr = malloc(sizeof(Vector));
    Vector a = { data, type_size, 0, cap };
    *vec_ptr = a;
    return (void**)vec_ptr;
}

void vec_free(void* vec, void(*free_fn)(byte*)) {
    Vector* v = (Vector*)vec;
    if(free_fn) {
        for(int i = 0; i < v->len * v->type_size; i+=v->type_size) {
            free_fn(*((void**)vec) + i);
        }
    }
    free(*((void**)vec));
    free(vec);
}

void vec_resize(Vector* vec, uint32 new_len) {
    uint32 len = vec->len > new_len ? new_len : vec->len;
    byte* data = malloc(new_len * vec->type_size * sizeof(byte));
    memcpy(data, vec->data, len*vec->type_size);
    free(vec->data);
    vec->data = data;
    vec->cap = new_len;
}

void vec_zap(void* vec, int _index, void(*free_fn)(byte*)) {
    Vector* v = (Vector*)vec;
    uint32 index = (_index >= 0) ? _index : (v->len + _index);
    if(free_fn) {
        free_fn(v->data + index * v->type_size);
    }
    uint32 copy_length = v->type_size * (v->len - index - 1);
    memcpy(v->data + (index * v->type_size), v->data + ((index+1) * v->type_size), copy_length);
    --v->len;
    if (v->cap >= VEC_INIT_SIZE * 2 && v->len <= v->cap / 2) {
        vec_resize(v, v->len);
    }
}
