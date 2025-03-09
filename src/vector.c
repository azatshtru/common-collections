#include "../headers/vector.h"

void* vec_allocate(uint32 cap, uint32 type_size) {
    byte* data = malloc(cap * type_size * sizeof(byte));
    byte* tmp = malloc(type_size * sizeof(byte));
    Vector* vec_ptr = malloc(sizeof(Vector));
    Vector a = { data, tmp, type_size, 0, cap };
    *vec_ptr = a;
    return (void*)vec_ptr;
}

void vec_free(void* vec, void(*free_fn)(void*)) {
    Vector* v = (Vector*)vec;
    if(free_fn) {
        for(int i = 0; i < v->len * v->type_size; i+=v->type_size) {
            free_fn(v->data + i);
        }
    }
    free(v->data);
    free(v->tmp);
    free(vec);
}

void vec_resize(Vector* vec, uint32 new_len) {
    uint32 len = vec->len > new_len ? new_len : vec->len;
    byte* data = malloc(new_len * vec->type_size * sizeof(byte));
    memmove(data, vec->data, len*vec->type_size);
    free(vec->data);
    vec->data = data;
    vec->cap = new_len;
}

int vec_zap(void* vec, int _index, void(*free_fn)(void*)) {
    Vector* v = (Vector*)vec;
    uint32 index = (_index >= 0) ? _index : (v->len + _index);
    if(free_fn) {
        free_fn(v->data + index * v->type_size);
    }
    uint32 copy_length = v->type_size * (v->len - index - 1);
    memmove(v->data + (index * v->type_size), v->data + ((index+1) * v->type_size), copy_length);
    --v->len;
    if (v->cap >= VEC_INIT_SIZE * 2 && v->len <= v->cap / 2) {
        vec_resize(v, v->len);
    }
    return 0;
}

int vec_cmp(void* v1_ptr, void* v2_ptr) {
    Vector* v1 = (Vector*)v1_ptr;
    Vector* v2 = (Vector*)v2_ptr;
    return v1->len == v2->len && !memcmp(v1->data, v2->data, v1->len);
}

uint32 vec_len(void* vec) {
    return ((Vector*)vec)->len;
}

int vec_shush_index(void* vec, void* value_ptr) {
    Vector* v = (Vector*)vec;
    for(int i = 0; i < v->len; i++) {
        if(!memcmp(v->data+i*v->type_size, v->tmp, v->type_size)) {
            return i;
        }
    }
    return -1;
}

void vec_clear(void* vec) {
    Vector* v = (Vector*)vec;
    v->len = 0;
    while(v->cap >= VEC_INIT_SIZE * 2 && v->len <= v->cap / 2) {
        vec_resize(v, v->len);
    }
}

void vec_reverse(void* vec) {
    Vector* v = (Vector*)vec;
    int size = v->type_size;
    for(int i = 0, j = v->len - 1; i < j; i++, j--) {
        char temp[size];
        memset(temp, 0, size);
        memmove(temp, v->data + i * size, size);
        memmove(v->data + i * size, v->data + j * size, size);
        memmove(v->data + j * size, temp, size);
    }
}

