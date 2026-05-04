#include <string.h>
#include <stdio.h>
#include "value.h"

#include "main.h"
#include "memory.h"
#include "continuation.h"

void print_list(struct Cell *c);
void print_value(value v) {
  if (!VALUEISAPOINTER(v))
    throw("value is not a pointer");
  switch (TYPEOF(v)) {
  case NUMBER: {
    float n = E_NUMBER(v);
    if (n - (float)(int)n == 0.0)
      printf("%d", (int)n);
    else
      printf("%f", n);
    break;
  }
  case SYMBOL:
    printf("%s", E_SYMBOL(v));
    break;
  case CELL:
    print_list(E_CELL(v));
    break;
  case LAMBDA:
    printf("LAMBDA ");
    print_list(E_LAMBDA(v)->args);
    break;
  case IFUNC:
    printf("IFUNC %p", E_IFUNC(v));
    break;
  case BOOLEAN:
    printf("%s", E_BOOLEAN(v) ? "#t" : "#f");
    break;
  case STRING:
    printf("%s", E_STRING(v));
    break;
  case CONTINUATION:
    printf("CONTINUATION");
    break;
  }
}
void print_list(struct Cell *c) {
  printf("(");
  if (CELL_IS_EMPTY(c)) goto end;
  print_value(CAR(c));
  printf(" . ");
  print_value(CDR(c));
end:
  printf(")");
}

value mk_number_value(float number) {
  struct Number *n = malloc(sizeof(struct Number));
  n->flags = 0;
  n->flags = (n->flags & ~TYPEMASK) | NUMBER;
  n->number = number;
  return (value) n;
}
value mk_symbol_value(char *symbol) {
  struct Symbol *s = malloc(sizeof(struct Symbol));
  s->flags = 0;
  s->flags = (s->flags & ~TYPEMASK) | SYMBOL;
  s->symbol = malloc(strlen(symbol) + 1);
  strcpy(s->symbol, symbol);
  return (value) s;
}
value mk_cell_value(value car, value cdr) {
  struct Cell *c = malloc(sizeof(struct Cell));
  c->flags = 0;
  c->flags = (c->flags & ~TYPEMASK) | CELL;
  c->car = car;
  c->cdr = cdr;
  return (value) c;
}
value mk_empty_cell_value() {
  struct Cell *c = malloc(sizeof(struct Cell));
  c->flags = 0;
  c->flags = (c->flags & ~TYPEMASK) | CELL;
  c->car = (value)NULL;
  c->cdr = (value)NULL;
  return (value) c;
}
value mk_lambda_value(struct Cell *args, value body, frame *env) {
  struct Lambda *l = malloc(sizeof(struct Lambda));
  l->flags = 0;
  l->flags = (l->flags & ~TYPEMASK) | LAMBDA;
  l->args = args;
  l->body = body;
  l->env  = env;
  return (value) l;
}
value mk_ifunc_value(ifunc f) {
  struct Ifunc *v = malloc(sizeof(struct Ifunc));
  v->flags = 0;
  v->flags = (v->flags & ~TYPEMASK) | IFUNC;
  v->func = f;
  return (value) v;
}
value mk_boolean_value(int b) {
  struct Boolean *v = malloc(sizeof(struct Boolean));
  v->flags = 0;
  v->flags = (v->flags & ~TYPEMASK) | BOOLEAN;
  v->boolean = b;
  return (value) v;
}
value mk_string_value(char *str) {
  struct String *s = malloc(sizeof(struct String));
  s->flags = 0;
  s->flags = (s->flags & ~TYPEMASK) | STRING;
  s->string = malloc(strlen(str) + 1);
  strcpy(s->string, str);
  return (value) s;
}
value mk_continuation_value() {
  struct Continuation *c = malloc(sizeof(struct Continuation));
  c->flags = 0;
  c->flags = (c->flags & ~TYPEMASK) | CONTINUATION;
  return (value) c;
}

// リストが()で終端されていない時の動作は未定義
int cell_len(struct Cell *c) {
  int len = 0;
  for(;;){
    len++;
    c = E_CELL(CDR(c));
    if (CELL_IS_EMPTY(c)) break;
  }
  return len;
}

int truish(value v) {
  // truish: E → T
  // truish = λε . (ε = false → false, true)
  if (TYPEOF(v) == BOOLEAN) {
    return E_BOOLEAN(v);
  }
  return 1;
}

// Deep comparison of value* (numbers, symbols, lists, etc.)
int value_equal(value a, value b) {
  if (a == b) return 1;
  if (!a || !b) return 0;
  if (TYPEOF(a) != TYPEOF(b)) return 0;
  switch (TYPEOF(a)) {
    case NUMBER:
      return E_NUMBER(a) == E_NUMBER(b);
    case SYMBOL:
      return strcmp(E_SYMBOL(a), E_SYMBOL(b)) == 0;
    case STRING:
      return strcmp(E_STRING(a), E_STRING(b)) == 0;
    case BOOLEAN:
      return E_BOOLEAN(a) == E_BOOLEAN(b);
    case CELL: {
      struct Cell *ca = E_CELL(a), *cb = E_CELL(b);
      if (!ca && !cb) return 1;
      if (!ca || !cb) return 0;
      return value_equal(CAR(ca), CAR(cb)) && value_equal(CDR(ca), CDR(cb));
    }
    default:
      return 0;
  }
}
