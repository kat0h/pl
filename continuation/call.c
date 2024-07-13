#include <setjmp.h>
#include <stdio.h>

unsigned long main_rsp;
char cont[10000000];

jmp_buf buf;

#define GETRSP(rsp) asm volatile ("mov %%rsp, %0" : "=r" (rsp));

void func2() {
  puts("func2");
  unsigned long rsp;
  GETRSP(rsp);
  printf("rsp: %lx\n", rsp);
  puts("end of func2");
}

void func() {
  puts("func1_1");
  func2();
}

int main() {
  unsigned long rsp;
  GETRSP(rsp);
  printf("rsp: %lx\n", rsp);
  func();
  return 0;
}
