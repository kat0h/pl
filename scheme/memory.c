#include "main.h"

// utils
size_t MEMP = 0;
void *MEM[1000000] = {0};
void *xmalloc(size_t size) {
  void *p = malloc(size);
  MEM[MEMP++] = p;
  if (MEMP == 1000000) {
    throw("Internal Error xmalloc");
  }
  if (p == NULL)
    throw("malloc failed");
  return p;
}

