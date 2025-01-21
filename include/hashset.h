#ifndef HASHSET
#define HASHSET

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNV_PRIME 16777619
#define FNV_OFFSET_BASIS 2166136261
#define HASHSET_INIT_SIZE 2

struct hashset {
    char* data;
    char* data_states;
    char* tmp;
    unsigned int typesize;
    unsigned int cardinality;
    unsigned int len;
    unsigned int (*hash_fn)(void*);
    int (*cmp_fn)(void*, void*);
    int load;
    float load_factor_upsize_threshold;
    float load_factor_downsize_threshold;
};

unsigned int hash_fnv1a(char* s, unsigned int length);
unsigned int hash_fnv1a_cstr(void* s);
int cmp_cstr(void* s1, void* s2);

#define generic_cmp(type, x, y) *(type*)x == *(type*)y
#define generic_hash(type, x) hash_fnv1a((char*)x, sizeof(type))

#define hashset(type) type**

void* hashset_allocate(unsigned int typesize, unsigned int (*hash_fn)(void*), int (*cmp_fn)(void*, void*));
void hashset_free(void* hashset_ptr, void free_fn(void*));
unsigned int hashset_entry_index(void* hashset_ptr, void* value_ptr, char is_inserting);
void hashset_shush_insert(void* hashset_ptr, void* value_ptr);
void hashset_chqnsrt(void* hashset_ptr, void* value_ptr);
void hashset_shush_remove(void* hashset_ptr, void* value_ptr);
unsigned int hashset_shush_contains(void* hashset_ptr, void* value_ptr);
unsigned int hashset_compute_resize(struct hashset* hashset);
void hashset_resize_rehash(struct hashset* hashset, unsigned int new_cardinality);

#define hashset_new(type, hash_fn, cmp_fn) hashset_allocate(sizeof(type), hash_fn, cmp_fn);
#define hashset_insert(hashset_ptr, value) (**((hashset_ptr)+2)=(value), hashset_chqnsrt(hashset_ptr, *((hashset_ptr)+2)))
#define hashset_remove(hashset_ptr, value) (**((hashset_ptr)+2)=(value), hashset_shush_remove(hashset_ptr, *((hashset_ptr)+2)))
#define hashset_contains(hashset_ptr, value) (**((hashset_ptr)+2)=(value), hashset_shush_contains(hashset_ptr, *((hashset_ptr)+2)))
#define hashset_get(hashset_ptr, value) (**((hashset_ptr)+2)=(value), (*(hashset_ptr))[hashset_entry_index(hashset_ptr, *((hashset_ptr)+2), 0)])

#define hashset_print_primitive(hashset_ptr, identifier)                         \
    do{                                                                          \
        printf("hashset![");                                                     \
        for (int i = 0; i < ((struct hashset *)(hashset_ptr))->cardinality; i++){\
            if (((struct hashset *)(hashset_ptr))->data_states[i] == 2){         \
                printf(identifier, (*(hashset_ptr))[i]);                         \
                printf(", ");                                                    \
            }                                                                    \
        }                                                                        \
        printf("\b\b]\n");                                                       \
    } while (0)

#define hashset_print(hashset_ptr, print_fn)                                     \
    do{                                                                          \
        printf("hashset![");                                                     \
        for (int i = 0; i < ((struct hashset *)(hashset_ptr))->cardinality; i++){\
            if (((struct hashset *)(hashset_ptr))->data_states[i] == 2){         \
                print_fn((*(hashset_ptr))[i]);                                   \
                printf(", ");                                                    \
            }                                                                    \
        }                                                                        \
        printf("\b\b]\n");                                                       \
    } while (0)


#endif
