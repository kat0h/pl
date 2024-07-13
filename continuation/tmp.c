#include <alloca.h>

#define GETRSP(rsp) asm volatile ("mov %%rsp, %0" : "=r" (rsp));

void f(int n) {
  
  void *p[n];
}
