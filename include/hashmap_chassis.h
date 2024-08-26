#ifndef HASHMAP_FUSELAGE
#define HASHMAP_FUSELAGE

#include <string.h>

#define HASHMAP_INIT_SIZE 1

typedef unsigned long uint64;
typedef unsigned int uint32;

typedef struct hashmap_chassis {
    char* values;
    char* keys;
    char* tombstones;
    unsigned int keysize;
    unsigned int valsize;
    unsigned int len;
    unsigned long cardinality;
    float load_factor_upsize_threshold;
    float load_factor_downsize_threshold;
} HashMapChassis;

#define __hashmap_impl__(alias, K, V, keyhash_fn, keycmp_fn)                            \
    struct alias                                                                        \
    {                                                                                   \
        K *tmp;                                                                         \
        V **basis;                                                                      \
    } alias##_default = {keyhash_fn, keycmp_fn, NULL, NULL};                            \
    struct alias alias##_init()                                                         \
    {                                                                                   \
        struct alias al = alias##_default;                                              \
        al.basis = hashmap_allocate(sizeof(K), sizeof(V));                              \
        al.tmp = NULL;                                                                  \
        return al;                                                                      \
    }                                                                                   \
    float alias##_keycmp(void *a, void *b)                                              \
    {                                                                                   \
        return keycmp_fn(*((K *)a), *((K *)b));                                         \
    }                                                                                   \
    unsigned long alias##_keyhash(void *a)                                              \
    {                                                                                   \
        return keyhash_fn(*((K *)a));                                                   \
    }

uint64 hash_fnv1a_str(char* key);
uint64 hash_fnv1a_int(long key);

void** hashmap_allocate(uint64 key_size, uint64 val_size);
void hashmap_free(void* hashmap_ptr, void(*key_free)(char*), void(*val_free)(char*));
void hashmap_rehash(void* hashmap_ptr, uint64 new_cardinality);
int hashmap_compute_resize(void* hashmap_ptr);

unsigned int hashmap_entry(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*));
int hashmap_insert_key(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*));
int hashmap_contains_key(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*));
int hashmap_remove_key(void* hashmap_ptr, void* key_ptr, unsigned int (*key_hash)(void*), float (*key_cmp)(void*, void*));

#define hashmap_insert(hashmap_ptr, key_ptr, value) (*(hashmap_ptr))[hashmap_insert_key((hashmap_ptr), key_ptr)] = (value)
#define hashmap_get(hashmap_ptr, key_ptr) (*(hashmap_ptr))[hashmap_contains_key((hashmap_ptr), key_ptr)]

#endif