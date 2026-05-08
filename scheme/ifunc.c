#include "ifunc.h"

// internal func
value ifunc_add(value args, frame *env) {
  float sum = 0;
  while (!CELL_IS_EMPTY(E_CELL(args))) { // TODO
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
  while (!CELL_IS_EMPTY(E_CELL(args))) {
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
  while (!CELL_IS_EMPTY(E_CELL(args))) {
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
  while (!CELL_IS_EMPTY(E_CELL(args))) {
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
  while (!CELL_IS_EMPTY(E_CELL(args))) {
    i = eval(CAR(E_CELL(args)), env);
    args = CDR(E_CELL(args));
  }
  return i;
}
value ifunc_define(value args, frame *env) {
  if (CELL_IS_EMPTY(E_CELL(args))) {
    throw("define error: no symbol");
  }
  if (TYPEOF(CAR(E_CELL(args))) != SYMBOL) {
    throw("define error: symbol is not symbol");
  }
  char *symbol = E_SYMBOL(CAR(E_CELL(args)));
  args = CDR(E_CELL(args));
  if (CELL_IS_EMPTY(E_CELL(args))) {
    throw("define error: too few arguments");
  }
  value value = eval(CAR(E_CELL(args)), env);
  // if (!CELL_IS_EMPTY(E_CELL(CDR(E_CELL(args))))) {
  //   throw("define error: too many arguments");
  // }
  return define_to_env(env, symbol, value);
}
value ifunc_setbang(value args, frame *env) {
  if (CELL_IS_EMPTY(E_CELL(args))) {
    throw("define error: no symbol");
  }
  if (TYPEOF(CAR(E_CELL(args))) != SYMBOL) {
    throw("define error: symbol is not symbol");
  }
  char *symbol = E_SYMBOL(CAR(E_CELL(args)));
  args = CDR(E_CELL(args));
  if (CELL_IS_EMPTY(E_CELL(args))) {
    throw("define error: too few arguments");
  }
  value value = eval(CAR(E_CELL(args)), env);
  // if (!CELL_IS_EMPTY(E_CELL(CDR(E_CELL(args))))) { // なぜか死ぬので無視
  //   throw("define error: too many arguments");
  // }
  return set_to_env(env, symbol, value);
}
value ifunc_showenv(value args, frame *env) {
  if (!CELL_IS_EMPTY(E_CELL(args))) {
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
  value first = CAR(E_CELL(args));
  if (!check_args(first)) throw("lambda error: args is not list of symbol");
  struct Cell *largs = E_CELL(first);
  value body = CAR(E_CELL(CDR(E_CELL(args))));
  return mk_lambda_value(largs, body, env);
}
value ifunc_print(value args, frame *env) {
  while (!CELL_IS_EMPTY(E_CELL(args))) {
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
  while (!CELL_IS_EMPTY(E_CELL(args))) {
    int i = truish(eval(CAR(E_CELL(args)), env));
    if (i == 0)
      return mk_boolean_value(0);
    args = CDR(E_CELL(args));
  }
  return mk_boolean_value(1);
}
value ifunc_or(value args, frame *env) {
  while (!CELL_IS_EMPTY(E_CELL(args))) {
    int i = truish(eval(CAR(E_CELL(args)), env));
    if (i)
      return mk_boolean_value(1);
    args = CDR(E_CELL(args));
  }
  return mk_boolean_value(0);
}

value ifunc_cond(value args, frame *env) {
  while (!CELL_IS_EMPTY(E_CELL(args))) {
    // (cond list* (else value*)?)
    // list = (value*)
    value list = CAR(E_CELL(args));
    if (TYPEOF(list) != CELL)
      throw("cond error: not list");
    value cond;
    while (!CELL_IS_EMPTY(E_CELL(list))) {
      if (TYPEOF(CAR(E_CELL(list))) == SYMBOL &&
          strcmp(E_SYMBOL(CAR(E_CELL(list))), "else") == 0) {
        // elseのあとをチェック
        if (!CELL_IS_EMPTY(E_CELL(CDR(E_CELL(args)))))
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
