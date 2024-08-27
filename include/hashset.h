#ifndef HASHSET
#define HASHSET

#define HASHSET_INIT_SIZE 1

struct hashset {
    char* data;
    char* data_states;
    char* tmp;
    unsigned int typesize;
    unsigned int cardinality;
    unsigned int len;
    unsigned int (*hash_fn)(void*);
    unsigned int (*cmp_fn)(void*, void*);
    float load_factor_upsize_threshold;
    float load_factor_downsize_threshold;
};

unsigned int hash_fnv1a_str(void* value_ptr);
unsigned int hash_fnv1a_int(void* value_ptr);

void* hashset_allocate(unsigned int typesize, unsigned int (*hash_fn)(void*), unsigned int (*cmp_fn)(void*, void*));
void hashset_free(void* hashset_ptr, void free_fn(void*));
unsigned int hashset_entry_index(void* hashset_ptr, void* value_ptr);
void hashset_shush_insert(void* hashset_ptr, void* value_ptr);
void hashset_chqnsrt(void* hashset_ptr, void* value_ptr);
unsigned int hashset_compute_resize(struct hashset* hashset);
void hashset_resize_rehash(struct hashset* hashset, unsigned int new_cardinality);

#define hashset_put(hashset_ptr, value) (**((hashset_ptr)+2)=(value), hashset_chqnsrt(hashset_ptr, *((hashset_ptr)+2)))

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