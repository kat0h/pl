#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "eval.h"

#include "main.h"
#include "env.h"
#include "continuation.h"

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
  while (fargs != NULL) {
    add_kv_to_frame(newenv, E_SYMBOL(fargs->car), eval(args->car, env));
    fargs = E_CELL(fargs->cdr);
    args = E_CELL(args->cdr);
  }
  return eval(f->body, newenv);
}

// internal func
value ifunc_add(value args, frame *env) {
  float sum = 0;
  while (E_CELL(args) != NULL) { // TODO
    value i = eval(CAR(E_CELL(args)), env);
    if (TYPEOF(i) != NUMBER)
      throw("add error: not number");
    sum += E_NUMBER(i);
    args = CDR(E_CELL(args));
  }
  return mk_number_value(sum);
}
value ifunc_sub(value args, frame *env) {
  value first = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(first) != NUMBER)
    throw("sub error: not number");
  float sum = E_NUMBER(first);
  if (cell_len(E_CELL(args)) == 1)
    return mk_number_value(-sum);
  args = CDR(E_CELL(args));
  while (E_CELL(args) != NULL) {
    value i = eval(CAR(E_CELL(args)), env);
    if (TYPEOF(i) != NUMBER)
      throw("sub error: not number");
    sum -= E_NUMBER(i);
    args = CDR(E_CELL(args));
  }
  return mk_number_value(sum);
}
value ifunc_mul(value args, frame *env) {
  float sum = 1.0;
  while (E_CELL(args) != NULL) {
    value i = eval(CAR(E_CELL(args)), env);
    if (TYPEOF(i) != NUMBER) {
      throw("mul error: not number");
    }
    sum *= E_NUMBER(i);
    args = CDR(E_CELL(args));
  }
  return mk_number_value(sum);
}
value ifunc_div(value args, frame *env) {
  float sum = 0;
  while (E_CELL(args) != NULL) {
    value i = eval(CAR(E_CELL(args)), env);
    if (TYPEOF(i) != NUMBER)
      throw("mul error: not number");
    if (E_NUMBER(i) == 0)
      throw("div error: zero division");
    sum /= E_NUMBER(i);
    args = CDR(E_CELL(args));
  }
  return mk_number_value(sum);
}
value ifunc_modulo(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 2)
    throw("modulo error: invalid number of arguments");
  value a = eval(CAR(E_CELL(args)), env);
  value b = eval(CAR(E_CELL(CDR(E_CELL(args)))), env);
  if (TYPEOF(a) != NUMBER || TYPEOF(b) != NUMBER)
    throw("modulo error: not number");
  int ia = (int)E_NUMBER(a);
  int ib = (int)E_NUMBER(b);
  return mk_number_value(ia % ib);
}
value ifunc_begin(value args, frame *env) {
  value i = mk_number_value(0);
  while (E_CELL(args) != NULL) {
    i = eval(CAR(E_CELL(args)), env);
    args = CDR(E_CELL(args));
  }
  return i;
}
value ifunc_define(value args, frame *env) {
  if (E_CELL(args) == NULL) {
    throw("define error: no symbol");
  }
  if (TYPEOF(CAR(E_CELL(args))) != SYMBOL) {
    throw("define error: symbol is not symbol");
  }
  char *symbol = E_SYMBOL(CAR(E_CELL(args)));
  args = CDR(E_CELL(args));
  if (E_CELL(args) == NULL) {
    throw("define error: too few arguments");
  }
  value value = eval(CAR(E_CELL(args)), env);
  if (E_CELL(CDR(E_CELL(args))) != NULL) {
    throw("define error: too many arguments");
  }
  return define_to_env(env, symbol, value);
}
value ifunc_setbang(value args, frame *env) {
  if (E_CELL(args) == NULL) {
    throw("define error: no symbol");
  }
  if (TYPEOF(CAR(E_CELL(args))) != SYMBOL) {
    throw("define error: symbol is not symbol");
  }
  char *symbol = E_SYMBOL(CAR(E_CELL(args)));
  args = CDR(E_CELL(args));
  if (E_CELL(args) == NULL) {
    throw("define error: too few arguments");
  }
  value value = eval(CAR(E_CELL(args)), env);
  if (E_CELL(CDR(E_CELL(args))) != NULL) {
    throw("define error: too many arguments");
  }
  return set_to_env(env, symbol, value);
}
value ifunc_showenv(value args, frame *env) {
  if (E_CELL(args) != NULL) {
    throw("showenv error: too many arguments");
  }
  print_frame(env);
  return mk_number_value(0);
}
int check_args(value args) {
  if (TYPEOF(args) != CELL) // argsはリストでないとならない
    return 0;
  struct Cell *cargs = E_CELL(args);
  if (CELL_IS_EMPTY(cargs)) // 空のリストはargsとして妥当
    return 1;
  if (TYPEOF(CAR(cargs)) != SYMBOL) // 各要素はSYMBOLでないとならない
    return 0;
  // 残りの要素も再帰的にチェック
  return check_args(CDR(E_CELL(args)));
}
value ifunc_lambda(value args, frame *env) {
  // (lambda (args) body)
  if (E_CELL(args) == NULL)
    throw("lambda error: no args");
  value first = CAR(E_CELL(args));
  if (!check_args(first))
    throw("lambda error: args is not list of symbol");
  struct Cell *largs = E_CELL(first);
  if (E_CELL(CDR(E_CELL(args))) == NULL)
    throw("lambda error: no body");
  value body = CAR(E_CELL(CDR(E_CELL(args))));
  if (E_CELL(CDR(E_CELL(CDR(E_CELL(args))))) != NULL)
    throw("lambda error: too many body");
  return mk_lambda_value(largs, body, env);
}
value ifunc_print(value args, frame *env) {
  while (E_CELL(args) != NULL) {
    print_value(eval(CAR(E_CELL(args)), env));
    puts("");
    args = CDR(E_CELL(args));
  }
  return mk_number_value(0);
}
value ifunc_if(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 2 && cell_len(E_CELL(args)) != 3)
    throw("if error: invalid number of arguments");
  value cond = eval(CAR(E_CELL(args)), env);
  if (truish(cond)) {
    return eval(CAR(E_CELL(CDR(E_CELL(args)))), env);
  } else {
    if (cell_len(E_CELL(args)) == 2)
      return mk_empty_cell_value();
    return eval(CAR(E_CELL(CDR(E_CELL(CDR(E_CELL(args)))))), env);
  }
}
value ifunc_quote(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 1)
    throw("quote error: invalid number of arguments");
  return CAR(E_CELL(args));
}
enum { EQ = 0, LT, LE, GT, GE };
int comp(value args, frame *env, char type) {
  int len = cell_len(E_CELL(args));
  if (len < 2)
    throw("comp error: too few arguments");
  value car = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(car) != NUMBER)
    throw("comp error: not number");
  value cdr;
  int result = 1;
  for (int i = 0; i < len - 1; i++) {
    cdr = eval(CAR(E_CELL(CDR(E_CELL(args)))), env);
    if (TYPEOF(cdr) != NUMBER)
      throw("comp error: not number");
    switch (type) {
    case EQ: // =
      result &= E_NUMBER(car) == E_NUMBER(cdr);
      break;
    case LT: // <
      result &= E_NUMBER(car) < E_NUMBER(cdr);
      break;
    case LE: // <=
      result &= E_NUMBER(car) <= E_NUMBER(cdr);
      break;
    case GT: // >
      result &= E_NUMBER(car) > E_NUMBER(cdr);
      break;
    case GE: // >=
      result &= E_NUMBER(car) >= E_NUMBER(cdr);
      break;
    }
    car = cdr;
    args = CDR(E_CELL(args));
  }
  return result;
}
value ifunc_eq(value args, frame *env) {
  return mk_boolean_value(comp(args, env, EQ));
}
value ifunc_lt(value args, frame *env) {
  return mk_boolean_value(comp(args, env, LT));
}
value ifunc_le(value args, frame *env) {
  return mk_boolean_value(comp(args, env, LE));
}
value ifunc_gt(value args, frame *env) {
  return mk_boolean_value(comp(args, env, GT));
}
value ifunc_ge(value args, frame *env) {
  return mk_boolean_value(comp(args, env, GE));
}
value ifunc_and(value args, frame *env) {
  while (E_CELL(args) != NULL) {
    int i = truish(eval(CAR(E_CELL(args)), env));
    if (i == 0)
      return mk_boolean_value(0);
    args = CDR(E_CELL(args));
  }
  return mk_boolean_value(1);
}
value ifunc_or(value args, frame *env) {
  while (E_CELL(args) != NULL) {
    int i = truish(eval(CAR(E_CELL(args)), env));
    if (i)
      return mk_boolean_value(1);
    args = CDR(E_CELL(args));
  }
  return mk_boolean_value(0);
}
value eval_list(value args, frame *env, value default_value) {
  value i = default_value;
  while (E_CELL(args) != NULL) {
    i = eval_top(CAR(E_CELL(args)), env);
    args = CDR(E_CELL(args));
  }
  return i;
}
value ifunc_cond(value args, frame *env) {
  while (E_CELL(args) != NULL) {
    // (cond list* (else value*)?)
    // list = (value*)
    value list = CAR(E_CELL(args));
    if (TYPEOF(list) != CELL)
      throw("cond error: not list");
    value cond;
    while (E_CELL(list) != NULL) {
      if (TYPEOF(CAR(E_CELL(list))) == SYMBOL &&
          strcmp(E_SYMBOL(CAR(E_CELL(list))), "else") == 0) {
        // elseのあとをチェック
        if (E_CELL(CDR(E_CELL(args))) != NULL)
          throw("cond error: else is not last");
        return eval_list(CDR(E_CELL(list)), env, mk_number_value(0));
      }
      cond = eval(CAR(E_CELL(list)), env);
      if (truish(cond))
        return eval_list(CDR(E_CELL(list)), env, cond);
      else
        break;
      list = CDR(E_CELL(list));
    }
    args = CDR(E_CELL(args));
  }
  return mk_empty_cell_value();
}
value ifunc_cons(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 2)
    fprintf(stderr, "cons error: invalid number of arguments");
  value car = eval(CAR(E_CELL(args)), env);
  value cdr = eval(CAR(E_CELL(CDR(E_CELL(args)))), env);
  return mk_cell_value(car, cdr);
}
value ifunc_car(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 1)
    throw("car error: invalid number of arguments");
  value c = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(c) != CELL)
    throw("car error: not pair");
  return CAR(E_CELL(c));
}
value ifunc_cdr(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 1)
    throw("car error: invalid number of arguments");
  value c = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(c) != CELL)
    throw("car error: not pair");
  return CDR(E_CELL(c));
}

value ifunc_rand(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 0) 
    throw("random error: arg");
  return mk_number_value(rand());
}

value ifunc_length(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 1)
    throw("length error: invalid number of arguments");
  value c = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(c) != CELL)
    throw("length error: not pair");
  return mk_number_value(cell_len(E_CELL(c)));
}

// プログラムの動作をn秒停止
value ifunc_sleep(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 1)
    throw("sleep error: invalid number of arguments");
  value c = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(c) != NUMBER)
    throw("sleep error: not number");
  sleep(E_NUMBER(c));
  return mk_number_value(0);
}

value ifunc_callcc(value args, frame *env) {
  if (cell_len(E_CELL(args)) != 1)
    throw("call/cc error: invalid number of arguments");
  value lmd = eval(CAR(E_CELL(args)), env);
  if (TYPEOF(lmd) != LAMBDA)
    throw("call/cc error: not lambda");
  value cont = mk_continuation_value();
  value r = get_continuation(cont);
  if ((void*)r == NULL) {
    // lambdaにcontinuationを渡して実行
    return eval_lambda(
        E_LAMBDA(lmd),
        E_CELL(mk_cell_value(cont, mk_empty_cell_value())),
        env);
  } else {
    // continuationが呼ばれた場合
    return r;
  }
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

