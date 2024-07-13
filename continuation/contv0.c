// this program works on GCC only (check in 14.1.1 linux)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

jmp_buf jmp;
typedef struct {
  void *stack;
  unsigned long stacklen;
  void *rsp;
  jmp_buf cont_reg;
} continuation[1];

void *main_rbp;
#define GETRSP(rsp) asm volatile ("mov %%rsp, %0" : "=r" (rsp));
#define GETRBP(rbp) asm volatile ("mov %%rbp, %0" : "=r" (rbp));
#define INIT_CONTINUATION() GETRBP(main_rbp)
int get_continuation(continuation c);
void call_continuation(continuation c);

continuation cont;
int func2() {
  if (get_continuation(cont) == 0) {
    puts("func2");
    return 0;
  } else {
    puts("func2 cont");
    return 1;
  }
}

int func() {
  return func2();
}

int main() {
  INIT_CONTINUATION();
  puts("main");
  int a = 0;
  if (func() == 0) {
    puts("main_1");
    printf("a: %d\n", a);
    a++;
    call_continuation(cont);
  } else {
    puts("main_2");
    printf("a: %d\n", a);
  }
}

int get_continuation(continuation c) {
  void *rsp;
  GETRSP(rsp);
  c->rsp = rsp;
  c->stacklen = main_rbp - rsp + 1;
  c->stack = malloc(sizeof(char) * c->stacklen);
  memcpy(c->stack, rsp, c->stacklen);
  return setjmp(c->cont_reg);
}

void call_continuation(continuation c) {
  memcpy(c->rsp, c->stack, c->stacklen);
  longjmp(c->cont_reg, 1);
}
