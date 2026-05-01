#ifndef EVAL_H
#define EVAL_H
#include "value.h"

value *eval(value *exp, frame *env);
value *eval_lambda(lambda *f, cell *args, frame *env);
value *eval_list(value *args, frame *env, value *default_value);

frame *mk_initial_env();
value *eval_top(value *exp, frame *env);

#endif

