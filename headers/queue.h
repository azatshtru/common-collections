#ifndef QUEUE
#define QUEUE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "miniqutils.h"

#define QUEUE_INIT_SIZE 2

typedef unsigned int uint32;

typedef struct queue {
    char* data;
    char* tmp;
    uint32 type_size;
    uint32 len;
    uint32 cap;
    uint32 front;
    uint32 back;
} Queue;

void** queue_allocate(uint32 cap, uint32 type_size);
void queue_free(void* queue_ptr, void(*free_fn)(char*));
void queue_resize(Queue* queue_ptr, uint32 new_len);
void queue_check_and_resize(Queue* q);
void queue_zap_front(void* queue_ptr, void(*free_fn)(char*));
void queue_zap_back(void* queue_ptr, void(*free_fn)(char*));
int queue_is_empty(void* queue_ptr);

#define VecDeque(type) type **
#define queue_new(type) (type **)queue_allocate(QUEUE_INIT_SIZE, sizeof(type))

#define queue_front(queue_ptr) (*(queue_ptr))[positive_mod(((Queue*)(queue_ptr))->front - 1, ((Queue*)(queue_ptr))->cap)]
#define queue_back(queue_ptr) (*(queue_ptr))[positive_mod(((Queue*)(queue_ptr))->back, ((Queue*)(queue_ptr))->cap)]

#define queue_pop_front(queue_ptr) (**((queue_ptr)+1)=queue_front((queue_ptr)), queue_zap_front((queue_ptr), NULL), **((queue_ptr)+1))
#define queue_pop_back(queue_ptr) (**((queue_ptr)+1)=queue_back((queue_ptr)), queue_zap_back((queue_ptr), NULL), **((queue_ptr)+1))

#define queue_push_front(queue_ptr, value)                          \
    do {                                                            \
        Queue* __q__ = (Queue*)(queue_ptr);                         \
        queue_check_and_resize(__q__);                              \
        (*(queue_ptr))[__q__->front] = (value);                     \
        __q__->front = positive_mod(__q__->front + 1, __q__->cap);  \
        ++__q__->len;                                               \
    } while(0)

#define queue_push_back(queue_ptr, value)                           \
    do {                                                            \
        Queue* __q__ = (Queue*)(queue_ptr);                         \
        queue_check_and_resize(__q__);                              \
        __q__->back = positive_mod(__q__->back - 1, __q__->cap);    \
        (*(queue_ptr))[__q__->back] = (value);                      \
        ++__q__->len;                                               \
    } while(0)

#define queue_print(queue_ptr, print_fn)                    \
    do {                                                    \
        Queue* __q__ = (Queue*)(queue_ptr);                 \
        printf("queue![");                                  \
        int j;                                              \
        for(int i = 0; i < __q__->len; i++) {               \
            j = positive_mod(__q__->back + i, __q__->cap);  \
            print_fn((*(queue_ptr))[j]);                    \
            printf(", ");                                   \
        }                                                   \
        printf("\b\b]\n");                                  \
    } while(0)

#define queue_print_primitive(queue_ptr, identifier)        \
    do {                                                    \
        Queue* __q__ = (Queue*)(queue_ptr);                 \
        printf("queue![");                                  \
        int j;                                              \
        for(int i = 0; i < __q__->len; i++) {               \
            j = positive_mod(__q__->back + i, __q__->cap);  \
            printf((identifier), (*(queue_ptr))[j]);        \
            printf(", ");                                   \
        }                                                   \
        printf("\b\b]\n");                                  \
    } while(0)

#endif

