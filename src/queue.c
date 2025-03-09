#include "../headers/queue.h"

void** queue_allocate(uint32 cap, uint32 type_size) {
    char* data = malloc(cap * type_size * sizeof(char));
    char* tmp = malloc(type_size * sizeof(char));
    Queue* q_ptr = malloc(sizeof(Queue));
    Queue q = { data, tmp, type_size, 0, cap, 0, 0 };
    *q_ptr = q;
    return (void**)q_ptr;
}

void queue_free(void* queue_ptr, void(*free_fn)(char*)) {
    Queue* q = (Queue*)queue_ptr;
    if(free_fn) {
        int j;
        for(int i = 0; i < q->len; i++) {
            j = positive_mod(q->back + i, q->cap);
            free_fn(q->data + j * q->type_size);
        }
    }
    free(q->data);
    free(q->tmp);
    free(q);
}

void queue_resize(Queue* q, uint32 new_len) {
    char* data = malloc(new_len * q->type_size * sizeof(char));
    if(q->front <= q->back) {
        int back_copy_size = (q->cap - q->back)*q->type_size;
        memcpy(data, q->data + q->back * q->type_size, back_copy_size);
        memcpy(data+back_copy_size, q->data, q->front*q->type_size);
    } else {
        memcpy(data, q->data + q->back * q->type_size, (q->front - q->back)*q->type_size);
    }
    free(q->data);
    q->data = data;
    q->cap = new_len;
    q->front = q->len;
    q->back = 0;
}

void queue_check_and_resize(Queue* q) {
    if(q->len >= q->cap) {
        queue_resize(q, q->cap * 2);
    } else if (q->cap >= QUEUE_INIT_SIZE * 2 && q->len <= q->cap / 2) {
        queue_resize(q, q->len);
    }
}

void queue_zap_front(void* queue_ptr, void(*free_fn)(char*)) {
    Queue* q = (Queue*)queue_ptr;
    if(free_fn) {
        free_fn(q->data + (q->front-1) * q->type_size);
    }
    q->front = positive_mod(q->front - 1, q->cap);
    --q->len;
    queue_check_and_resize(q);
}

void queue_zap_back(void* queue_ptr, void(*free_fn)(char*)) {
    Queue* q = (Queue*)queue_ptr;
    if(free_fn) {
        free_fn(q->data + q->back * q->type_size);
    }
    q->back = positive_mod(q->back + 1, q->cap);
    --q->len;
    queue_check_and_resize(q);
}

int queue_is_empty(void* queue_ptr) {
    Queue* q = (Queue*)queue_ptr;
    return q->len == 0;
}

