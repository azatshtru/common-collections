#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hashset.h"

void* hashset_allocate(unsigned int typesize, unsigned int (*hash_fn)(void*), unsigned int (*cmp_fn)(void*, void*)) {
    struct hashset* hashset_ptr = malloc(sizeof(struct hashset)); 
    char* data = malloc(typesize * HASHSET_INIT_SIZE);
    char* data_states = malloc(HASHSET_INIT_SIZE);
    memset(data_states, 0, HASHSET_INIT_SIZE);
    char* tmp = malloc(typesize);
    struct hashset hashset = { 
        data, data_states, tmp,
        typesize, HASHSET_INIT_SIZE, 0,
        hash_fn, cmp_fn,
        0.75, 0.1
    };
    *hashset_ptr = hashset;
    return (void*)hashset_ptr;
}

void hashset_free(void* hashset_ptr, void free_fn(void*)) {
    struct hashset* hashset = (struct hashset*)hashset_ptr;
    if(free_fn) {
        for(int i = 0; i < hashset->cardinality; i++) {
            if(hashset->data_states[i]) {
                free_fn(hashset->data + i*hashset->typesize);
            }
        }
    }
    free(hashset->data);
    free(hashset->tmp);
    free(hashset->data_states);
    free(hashset);
}

unsigned int hashset_entry_index(void* hashset_ptr, void* value_ptr) {
    int c1 = 0;
    int c2 = 1;
    struct hashset* hashset = (struct hashset*)hashset_ptr;
    unsigned int hash = hashset->hash_fn(value_ptr) % hashset->cardinality;
    int i = 1;
    while(hashset->data_states[hash] != 0) {
        if(hashset->data_states[hash] == 2 && hashset->cmp_fn(hashset->data+(hash*hashset->typesize), value_ptr)) {
            break;
        }
        hash = (hash + c1*i + c2*i*i) % hashset->cardinality;
        ++i;
    }
    return hash;
}

void hashset_shush_insert(void* hashset_ptr, void* value_ptr) {
    struct hashset* hashset = (struct hashset*)hashset_ptr;
    unsigned int hash = hashset_entry_index(hashset_ptr, value_ptr);
    hashset->data_states[hash] = 2;
    memcpy(hashset->data+hash*hashset->typesize, value_ptr, hashset->typesize);
    ++hashset->len;
}

void hashset_chqnsrt(void* hashset_ptr, void* value_ptr) {
    struct hashset* hashset = (struct hashset*)hashset_ptr;
    unsigned int rehash = hashset_compute_resize(hashset);
    if(rehash) {
        hashset_resize_rehash(hashset, rehash);
    }
    hashset_shush_insert(hashset_ptr, value_ptr);
}

unsigned int hashset_compute_resize(struct hashset* hashset) {
    float load_factor = (float)hashset->len / hashset->cardinality;
    if(load_factor >= hashset->load_factor_upsize_threshold) {
        return hashset->cardinality*2;
    } else if (load_factor < hashset->load_factor_downsize_threshold && hashset->cardinality >= 2 * HASHSET_INIT_SIZE) {
        return hashset->cardinality/2;
    }
    return 0;
}

void hashset_resize_rehash(struct hashset* hashset, unsigned int new_cardinality) {
    unsigned int old_cardinality = hashset->cardinality;
    char* old_data = hashset->data;
    char* old_data_states = hashset->data_states;

    hashset->data = malloc(hashset->typesize * new_cardinality);
    hashset->data_states = malloc(new_cardinality);
    memset(hashset->data_states, 0, new_cardinality);
    hashset->cardinality = new_cardinality;
    hashset->len = 0;

    for(int i = 0; i < old_cardinality; i++) {
        if(old_data_states[i] == 2) {
            hashset_shush_insert(hashset, old_data+(i*hashset->typesize));
        }
    }

    free(old_data);
    free(old_data_states);
}