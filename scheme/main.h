#ifndef MAIN_H
#define MAIN_H

#include "value.h"
#define SYMBOL_LEN_MAX 256

#include <stdlib.h>
#include <stdio.h>
// https://github.com/tadd/my-c-lisp
#define throw(fmt, ...)                                                        \
  {                                                                            \
    fprintf(stderr, "%s:%d of %s: " fmt "\n", __FILE__, __LINE__,              \
            __func__ __VA_OPT__(, ) __VA_ARGS__);                              \
    exit(1);                                                                   \
  }


value *eval(value *exp, frame *env);
value *eval_lambda(lambda *f, cell *args, frame *env);

frame *mk_initial_env();
value *eval_top(value *exp, frame *env);

#endif
