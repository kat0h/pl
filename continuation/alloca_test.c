#include <alloca.h>
#include <stdio.h>
#define GETRSP(rsp) asm volatile("mov %%rsp, %0" : "=r"(rsp));

int main() {
  void *rsp;
  GETRSP(rsp);
  printf("%p\n", rsp);
  int *p = alloca(312);
  void *nrsp;
  GETRSP(nrsp);
  printf("%p\n", nrsp);

  printf("rsp-nrsp: %ld\n", (long)rsp - (long)nrsp);
  return 0;
}
