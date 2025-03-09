#include <limits.h>
#include <string.h>
#include <stdarg.h>
#include "vector.h"

#ifndef MINIQUTILS
#define MINIQUTILS

#define EULER_CONSTANT 2.718281828459045235
#define PI 3.14159265358979323846

union raw_float {
    unsigned int u;
    float f;
};

typedef unsigned int Color;
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define BLACK 0x000000
#define WHITE 0xFFFFFF

double min_f(int n, ...);
int min_i(int n, ...);
void memzero(void* array, int len);
int power(int base, int exponent);
void swap(void* a, void* b, unsigned int size);
int log_base_2(int x);
int cantor_pairing(int a, int b);
float positive_infinity();
float negative_infinity();
int euclid_gcd(int a, int b);
int modpow(int x, int n, int m);
int positive_mod(int a, int m);
int close_to_zero(float epsilon);
double natural_log(double x);

#define I32_MIN -2147483648
#define I32_MAX 2147483647

#define container_of(ptr, type, member_name) ((type*)((char*)ptr - (unsigned long)(&(((type*)0)->member_name))))

#endif

