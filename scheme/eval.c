#include <stdio.h>
#include "eval.h"

#include "main.h"
#include "env.h"
#include "continuation.h"
#include "ifunc.h"

// eval
value eval_cell(struct Cell *exp, frame *env);
value eval(value exp, frame *env) {
  // expの型によって処理を振り分ける
  switch (TYPEOF(exp)) {
  case NUMBER:
    // NUMBERは値である
    return exp;
  case SYMBOL:
    // 環境からSYMBOLの値を探す
    return lookup_frame(env, E_SYMBOL(exp));
  case CELL:
    return eval_cell(E_CELL(exp), env);
  case LAMBDA:
    // LAMBDAは値である
    return exp;
  case IFUNC:
    // IFUNCは値である
    return exp;
  case BOOLEAN:
    // BOOLEANは値である
    return exp;
  case STRING:
    // STRINGは値である
    return exp;
  case CONTINUATION:
    // CONTINUATIONは値である
    return exp;
  }
  throw("Unreachable");
}

value eval_top(value exp, frame *env) {
  INIT_CONTINUATION();
  return eval(exp, env);
}

value eval_lambda(struct Lambda *f, struct Cell *args, frame *env);
value eval_cell(struct Cell *cell, frame *env) {
  if (CELL_IS_EMPTY(cell)) return VALUE(cell); // ()は値である
  value func = eval(CAR(cell), env);
  value args = CDR(cell);
  if (TYPEOF(func) == IFUNC) {
    return E_IFUNC(func)(args, env);
  } else if (TYPEOF(func) == LAMBDA) {
    if (TYPEOF(args) != CELL) throw("eval error: args is not CELL");
    return eval_lambda(E_LAMBDA(func), E_CELL(args), env);
  } else if (TYPEOF(func) == CONTINUATION) {
    continuation *cont = E_CONTINUATION(func);
    if (cell_len(E_CELL(args)) > 1) throw("call/cc error: invalid number of arguments"); // 引数の数は0か1でなければならない
    if (cell_len(E_CELL(args)) == 0)
      call_continuation(cont, mk_empty_cell_value());
    call_continuation(cont, eval(CAR(E_CELL(args)), env));
  }
  throw("call error: not callable");
}
value eval_lambda(struct Lambda *f, struct Cell *args, frame *env) {
  frame *newenv = make_frame(f->env);
  struct Cell *fargs = f->args;
  int fargc = cell_len(fargs);
  int argc = cell_len(args);
  if (fargc != argc)
    throw("lambda error: argument count mismatch expect %d but got %d", fargc,
          argc);
  while (!CELL_IS_EMPTY(fargs)) {
    add_kv_to_frame(newenv, E_SYMBOL(fargs->car), eval(args->car, env));
    fargs = E_CELL(fargs->cdr);
    args = E_CELL(args->cdr);
  }
  return eval(f->body, newenv);
}

value eval_list(value args, frame *env, value default_value) {
  value i = default_value;
  while (!CELL_IS_EMPTY(E_CELL(args))) {
    i = eval_top(CAR(E_CELL(args)), env);
    args = CDR(E_CELL(args));
  }
  return i;
}

// main
frame *mk_initial_env() {
  frame *env = make_frame(NULL);
  add_kv_to_frame(env, "+", mk_ifunc_value(ifunc_add));
  add_kv_to_frame(env, "-", mk_ifunc_value(ifunc_sub));
  add_kv_to_frame(env, "*", mk_ifunc_value(ifunc_mul));
  add_kv_to_frame(env, "/", mk_ifunc_value(ifunc_div));
  add_kv_to_frame(env, "modulo", mk_ifunc_value(ifunc_modulo));
  add_kv_to_frame(env, "begin", mk_ifunc_value(ifunc_begin));
  add_kv_to_frame(env, "define", mk_ifunc_value(ifunc_define));
  add_kv_to_frame(env, "set!", mk_ifunc_value(ifunc_setbang));
  add_kv_to_frame(env, "showenv", mk_ifunc_value(ifunc_showenv));
  add_kv_to_frame(env, "lambda", mk_ifunc_value(ifunc_lambda));
  add_kv_to_frame(env, "print", mk_ifunc_value(ifunc_print));
  add_kv_to_frame(env, "if", mk_ifunc_value(ifunc_if));
  add_kv_to_frame(env, "quote", mk_ifunc_value(ifunc_quote));
  add_kv_to_frame(env, "=", mk_ifunc_value(ifunc_eq));
  add_kv_to_frame(env, "<", mk_ifunc_value(ifunc_lt));
  add_kv_to_frame(env, "<=", mk_ifunc_value(ifunc_le));
  add_kv_to_frame(env, ">", mk_ifunc_value(ifunc_gt));
  add_kv_to_frame(env, ">=", mk_ifunc_value(ifunc_ge));
  add_kv_to_frame(env, "and", mk_ifunc_value(ifunc_and));
  add_kv_to_frame(env, "or", mk_ifunc_value(ifunc_or));
  add_kv_to_frame(env, "cond", mk_ifunc_value(ifunc_cond));
  add_kv_to_frame(env, "cons", mk_ifunc_value(ifunc_cons));
  add_kv_to_frame(env, "call/cc", mk_ifunc_value(ifunc_callcc));
  add_kv_to_frame(env, "car", mk_ifunc_value(ifunc_car));
  add_kv_to_frame(env, "cdr", mk_ifunc_value(ifunc_cdr));
  add_kv_to_frame(env, "rand", mk_ifunc_value(ifunc_rand));
  add_kv_to_frame(env, "length", mk_ifunc_value(ifunc_length));
  add_kv_to_frame(env, "sleep", mk_ifunc_value(ifunc_sleep));
  return env;
}

