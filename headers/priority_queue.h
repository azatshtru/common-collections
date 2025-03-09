#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include <stdlib.h>
#include <stdio.h>
#include "miniqutils.h"

#define PRIORITY_QUEUE_INIT_SIZE 1

struct priority_queue {
    char* data;
    char* tmp;
    int* priority;
    int len;
    int cap;
    int typesize;
};

void* priority_queue_allocate(unsigned int cap, unsigned long typesize);
void priority_queue_free(void* q_ptr, void(*free_fn)(void*));
void priority_queue_resize(void* q_ptr, int len);

int priority_queue_len(void* q_ptr);
int priority_queue_is_empty(void* q_ptr);

int priority_queue_zap_min(void* q_ptr, void(*free_fn)(void*));

void priority_queue_heapify_down(void* q_ptr);
void priority_queue_heapify_up(void* q_ptr);
int priority_queue_priority(void* q_ptr, int index);
void priority_queue_swap(void* q_ptr, int a, int b);

#define PriorityQueue(type) type**
#define priority_queue_new(type) priority_queue_allocate(PRIORITY_QUEUE_INIT_SIZE, sizeof(type))
#define priority_queue_min(q_ptr) (*q_ptr)[0]
#define priority_queue_get(q_ptr, index) (*q_ptr)[index]

#define priority_queue_nq(qptr, value, _priority)                                \
    do {                                                                        \
        struct priority_queue *__q__ = (struct priority_queue *)(qptr);         \
        if (__q__->len >= __q__->cap) {                                                 \
            priority_queue_resize(__q__, __q__->cap * 2);                               \
        }                                                                       \
        (*qptr)[__q__->len] = value;                                          \
        (__q__->priority)[__q__->len] = _priority;                               \
        ++__q__->len;                                                           \
        priority_queue_heapify_up(qptr);                                        \
    } while (0)

#define priority_queue_dq(queue) (**(queue+1) = **(queue), priority_queue_zap_min(queue, NULL), **(queue+1))


#endif

