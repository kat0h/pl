#include <stdlib.h>
#include <setjmp.h>
#include "continuation.h"
static void *main_rbp;

void init_continuation(void *rbp) {
  main_rbp = rbp;
}
int get_continuation(continuation *c) {
  void *rsp;
  GETRSP(rsp);
  c->rsp = rsp;
  c->stacklen = main_rbp - rsp + 1;
  c->stack = malloc(sizeof(char) * c->stacklen);
  char *dst = c->stack;
  char *src = c->rsp;
  for (int i = c->stacklen; 0 <= --i;)
    *dst++ = *src++;
  return setjmp(c->cont_reg);
}
void _cc(continuation *c, int val) {
  char *dst = c->rsp;
  char *src = c->stack;
  for (int i = c->stacklen; 0 <= --i;)
    *dst++ = *src++;
  longjmp(c->cont_reg, val);
}
void call_continuation(continuation *c, int val) {
  volatile void *q = 0;
  do {
    q=alloca(8);
  } while (q > c->rsp);
  _cc(c, val);
}
void free_continuation(continuation *c) {
  free(c->stack);
}
