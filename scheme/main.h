#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#define SYMBOL_LEN_MAX 256

// https://github.com/tadd/my-c-lisp
#define throw(fmt, ...)                                                        \
  {                                                                            \
    fprintf(stderr, "%s:%d of %s: " fmt "\n", __FILE__, __LINE__,              \
            __func__ __VA_OPT__(, ) __VA_ARGS__);                              \
    exit(1);                                                                   \
  }

#define TYPEOF(x) (x->type)
#define E_NUMBER(x) (x->body.number)
#define E_SYMBOL(x) (x->body.symbol)
#define E_CELL(x) (x->body.cell)
#define E_LAMBDA(x) (x->body.lmd)
#define E_IFUNC(x) (x->body.func)
#define E_BOOLEAN(x) (x->body.boolean)
#define E_STRING(x) (x->body.string)
#define E_CONTINUATION(x) (x->body.cont)
#define CAR(x) (E_CELL(x)->car)
#define CDR(x) (E_CELL(x)->cdr)

void *xmalloc(size_t size);
// environment
struct Frame {
  frame *parent;
  kv *kv;
};
struct KV {
  char *key;
  value *value;
  kv *next;
};

value *eval(value *exp, frame *env);
value *eval_lambda(lambda *f, cell *args, frame *env);

frame *mk_initial_env();
value *eval_top(value *exp, frame *env);

#endif
