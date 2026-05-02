#ifndef CONTINUATION_H
#define CONTINUATION_H
#include <setjmp.h>
#include "value.h"

typedef struct Continuation continuation;
void init_continuation(void *rbp);

#ifdef __x86_64__    
#define GETRSP(rsp) asm volatile("mov %%rsp, %0" : "=r"(rsp));
#define GETRBP(rbp) asm volatile("mov %%rbp, %0" : "=r"(rbp));
#else
#error "ARM is not supported"
#endif

#define INIT_CONTINUATION()                                                    \
  {                                                                            \
    void *main_rbp;                                                            \
    GETRBP(main_rbp);                                                          \
    init_continuation(main_rbp);                                               \
  }

void *get_continuation(value *cont);
void call_continuation(continuation *c, void *value);
void free_continuation(continuation *c);

#endif
