# lemonade
A juicy library of generic collections, all implemented in and for ANSI C.

lemonade includes vectors, hashmaps, strings, queues and priority_queues. Additionally, there is a utility library containing common functionality.

## Examples and sample usage
### Vectors
```c
#include <stdio.h>
#include "vector.h"

int main() {
    vector(int) v = vec_new(int);
    vec_push(v, 42);
    vec_push(v, 67);
    vec_push(v, 29);
    vec_push(v, 21);

    vec_print_primitive(v, "%d"); // prints [42, 67, 29, 21]
    int x = vec_pop(v, -2); // pop the second-to-last element
    printf("%d", x); // prints 29
    vec_print_primitive(v, "%d"); // prints [42, 67, 21]

    return 0;
}
```
### Hashmaps
There is a sample patient database created using hashmaps given in [Examples/patient_database.c]().

## API
### Vectors
1. `vector(type)` declarator for vector of given type
2. `vec_new(type)` creates a new vector for the given type
3. `vec_push(v, value)` pushes `value` at the end of vector `v`
4. `vec_insert(v, i, value)` inserts `value` at index `i`
5. `vec_pop(v, i)` removes the element at index `i` from vector `v` and returns it, also supports python like negative indexing.
6. `vec_zap(v, i)` removes the element at index `i`, doesn't return it, more safer to use, also supports python like negative indexing.
7. `vec_set(v, i, x)` sets the value at `i`th position to be `x`
8. `vec_get(v, i)` get the value at `i`th position
9. `vec_clear(v)` clears the vector
10. `vec_as_array(v)` returns the vector as an array allowing square bracket mutations
11. `vec_print` takes the vector and a print function for each element, then prints each element of the vector using the print function.
12. `vec_print_primitive` takes the primitive-typed vector and a type specifier, then prints the values in the vector
13. `vec_free(v, free_fn)` deallocates the memory allocated by the vector, if `free_fn` specified, then individual elements are freed using it.

### Hashsets
1. `hashset(type)`
2. `hashset_new(type, hash_function, compare_function)`
3. `hashset_insert(h, value)`
4. `hashset_remove(h, value)`
5. `hashset_get(h, value)`
6. `hashset_contains(h, value)`
7. `hashset_clone(h)`
8. `hashset_lazy_iterate(h)`
9. `hashset_free(h, free_fn)`

### Strings

### Queues

### Priority Queues
