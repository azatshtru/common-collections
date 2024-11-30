#include "include/vector.h"
#include "include/hashset.h"
#include <stdio.h>

typedef struct patient {
  char* name;
  unsigned char age;
  char* disease;
} Patient;

unsigned int patient_hash(void* a) {
  return hash_fnv1a_cstr((*(Patient*)a).name);
}

int patient_cmp(void* a, void* b) {
  Patient p1 = *(Patient*)a;
  Patient p2 = *(Patient*)b;

  return !strcmp(p1.name, p2.name);
}

Patient patient_new(char* name, int age, char* disease) {
  Patient p = { name, age, disease };
  return p;
}

Patient patient_key(char* name) {
  Patient p = { name, 0, "" };
  return p;
}

void patient_print(Patient p) {
  printf("\n  %s { age: %d, disease: %s }", p.name, p.age, p.disease);
}

int main() {
  hashset(Patient) h = hashset_new(Patient, patient_hash, patient_cmp);
  hashset_insert(h, patient_new("Adam", 16, "serotonin storm"));
  hashset_insert(h, patient_new("Daniel Bresson", 29, "Wiskott-Aldrich"));
  hashset_insert(h, patient_new("Carly Forlano", 32, "Congestive heart failure"));
  hashset_insert(h, patient_new("Rebecca Adler", 29, "Neurocysticercosis"));

  Patient p = hashset_get(h, patient_key("Adam"));

  hashset_print(h, patient_print);
  patient_print(p);

  hashset_free(h, NULL);

  return 0;
}
