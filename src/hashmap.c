#include <stdlib.h>
#include "../include/hashmap_chassis.h"

void** hashmap_allocate(uint64 key_size, uint64 val_size) {
    HashMapChassis* hashmap_ptr = malloc(sizeof(HashMapChassis)); 
    char* keys = malloc(key_size * HASHMAP_INIT_SIZE);
    char* values = malloc(val_size * HASHMAP_INIT_SIZE);
    char* tombstones = malloc(HASHMAP_INIT_SIZE);
    memset(tombstones, 0, HASHMAP_INIT_SIZE);
    HashMapChassis hashmap = { values, keys, tombstones, 
        key_size, val_size, 
        0, HASHMAP_INIT_SIZE,
        0.75, 0.1
    };
    *hashmap_ptr = hashmap;
    return (void**)hashmap_ptr;
}

void hashmap_free(void* hashmap_ptr, void(*key_free)(char*), void(*val_free)(char*)) {
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    if(key_free) {
        for(int i = 0; i < hashmap->len * hashmap->keysize; i+=hashmap->keysize) {
            key_free(hashmap->keys + i);
        }
    }
    if(val_free) {
        for(int i = 0; i < hashmap->len * hashmap->valsize; i+=hashmap->valsize) {
            val_free(hashmap->values + i);
        }
    }
    free(hashmap->keys);
    free(hashmap->values);
    free(hashmap->tombstones);
    free(hashmap);
}

unsigned int hashmap_entry(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*)) {
    int c1 = 0;
    int c2 = 1;
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    unsigned int hash = key_hash(key_ptr);
    int i = 1;
    while(hashmap->tombstones[hash] != 0) {
        if(hashmap->tombstones[hash] == 2 && key_cmp(hashmap->keys+(hash*hashmap->keysize), key_ptr)) {
            break;
        }
        hash = (hash + c1*i + c2*i*i) % hashmap->cardinality;
    }
    return hash;
}

void hashmap_rehash(void* hashmap_ptr, uint64 new_cardinality) {
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    char* keys = malloc(hashmap->keysize * new_cardinality);
    char* values = malloc(hashmap->valsize * new_cardinality);

}

int hashmap_compute_resize(void* hashmap_ptr) {
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    float load_factor = (float)hashmap->len / hashmap->cardinality;
    if(load_factor >= hashmap->load_factor_upsize_threshold) {
        return hashmap->cardinality*2;
    } else if (load_factor < hashmap->load_factor_downsize_threshold && hashmap->cardinality >= 2 * HASHMAP_INIT_SIZE) {
        return hashmap->cardinality/2;
    }
    return 0;
}

int hashmap_insert_key(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*)) {
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    unsigned int hash = hashmap_entry(hashmap_ptr, key_ptr, key_hash, key_cmp);
    hashmap->tombstones[hash] = 2;
    memcpy(hashmap->keys+hash*hashmap->keysize, key_ptr, hashmap->keysize);
    return hash;
}

int hashmap_contains_key(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*)) {
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    unsigned int hash = hashmap_entry(hashmap_ptr, key_ptr, key_hash, key_cmp);
    if(hashmap->tombstones[hash] == 0) {
        return -1;
    }
    return hash;
}

int hashmap_remove_key(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*)) {
    HashMapChassis* hashmap = (HashMapChassis*)hashmap_ptr;
    unsigned int hash = hashmap_entry(hashmap_ptr, key_ptr, key_hash, key_cmp);
    hashmap->tombstones[hash] = 1;
    return hash;
}