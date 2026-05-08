#ifndef IFUNC_H
#define IFUNC_H
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "value.h"
#include "eval.h"
#include "env.h"
#include "continuation.h"

// internal func
value ifunc_add(value args, frame *env);
value ifunc_sub(value args, frame *env);
value ifunc_mul(value args, frame *env);
value ifunc_div(value args, frame *env);
value ifunc_modulo(value args, frame *env);
value ifunc_begin(value args, frame *env);
value ifunc_define(value args, frame *env);
value ifunc_setbang(value args, frame *env);
value ifunc_showenv(value args, frame *env);
value ifunc_lambda(value args, frame *env);
value ifunc_print(value args, frame *env);
value ifunc_if(value args, frame *env);
value ifunc_quote(value args, frame *env);
value ifunc_eq(value args, frame *env);
value ifunc_lt(value args, frame *env);
value ifunc_le(value args, frame *env);
value ifunc_gt(value args, frame *env);
value ifunc_ge(value args, frame *env);
value ifunc_and(value args, frame *env);
value ifunc_or(value args, frame *env);
value ifunc_cond(value args, frame *env);
value ifunc_cons(value args, frame *env);
value ifunc_car(value args, frame *env);
value ifunc_cdr(value args, frame *env);
value ifunc_rand(value args, frame *env);
value ifunc_length(value args, frame *env);
value ifunc_sleep(value args, frame *env);
value ifunc_callcc(value args, frame *env);
enum { EQ = 0, LT, LE, GT, GE };
int comp(value args, frame *env, char type);
int check_args(value args);

#endif
