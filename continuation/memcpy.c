#include <setjmp.h>
#include <stdio.h>

#define GETRSP(rsp) asm volatile ("mov %%rsp, %0" : "=r" (rsp));
int main() {
  unsigned long rsp;
  printf("rsp: %lx\n", rsp);
  return 0;
}

