#include "../headers/priority_queue.h"

void* priority_queue_allocate(unsigned int cap, unsigned long typesize) {
    struct priority_queue* q = malloc(sizeof(struct priority_queue));
    q->data = malloc(PRIORITY_QUEUE_INIT_SIZE * typesize);
    q->priority = malloc(PRIORITY_QUEUE_INIT_SIZE * sizeof(int));
    q->tmp = malloc(typesize);
    q->len = 0;
    q->cap = cap;
    q->typesize = typesize;
    return q;
}

void priority_queue_free(void* q_ptr, void(*free_fn)(void*)) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    if(free_fn) {
        for(int i = 0; i < q->len; i++) {
            free_fn(q->data + i*q->typesize);
        }
    }
    free(q->data);
    free(q->priority);
    free(q->tmp);
    free(q);
}

void priority_queue_resize(void* q_ptr, int new_len) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    int len = q->len > new_len ? new_len : q->len;


    int* priority = malloc(new_len * sizeof(int));
    memmove(priority, q->priority, len * sizeof(int));
    char* data = malloc(new_len * q->typesize);
    memmove(data, q->data, len * q->typesize);

    free(q->data);
    free(q->priority);

    q->data = data;
    q->priority = priority;

    q->cap = new_len;
}

int priority_queue_len(void* q_ptr) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    return q->len;
}

int priority_queue_is_empty(void* q_ptr) {
    return priority_queue_len(q_ptr) == 0;
}

int priority_queue_priority(void* q_ptr, int index) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    return q->priority[index];
}

int priority_queue_zap_min(void* q_ptr, void(*free_fn)(void*)) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    priority_queue_swap(q, 0, priority_queue_len(q) - 1);
    if(free_fn) {
        free_fn(q->data + (q->len-1) * q->typesize);
    }
    --q->len;
    if (q->cap >= PRIORITY_QUEUE_INIT_SIZE * 2 && q->len <= q->cap / 2) {
        priority_queue_resize(q, q->cap/2);
    }
    priority_queue_heapify_down(q);
    return 0;
}

void priority_queue_heapify_up(void* q_ptr) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    int i = q->len - 1;

    while(i > 0) {
        int j = (i - 1) / 2;
        if (q->priority[i] < q->priority[j]) {
            priority_queue_swap(q, i, j);
            i = j;
        } else {
            break;
        }
    }
}

void priority_queue_heapify_down(void* q_ptr) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    int typesize = q->typesize;

    int i = 0;
    while(1) {
        int j = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < q->len && q->priority[left] < q->priority[j]) { j = left; }
        if (right < q->len && q->priority[right] < q->priority[j]) { j = right; }
        if (j == i) { break; }

        priority_queue_swap(q, i, j);
        i = j;
    }
}

void priority_queue_swap(void* q_ptr, int a, int b) {
    struct priority_queue* q = (struct priority_queue*)q_ptr;
    int typesize = q->typesize;
    swap(q->data + a*typesize, q->data + b*typesize, typesize);
    q->priority[a] = q->priority[a] ^ q->priority[b];
    q->priority[b] = q->priority[a] ^ q->priority[b];
    q->priority[a] = q->priority[a] ^ q->priority[b];
}

