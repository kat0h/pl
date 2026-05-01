#include <string.h>
#include <stdio.h>
#include "value.h"

#include "memory.h"

void print_list(cell *c);
void print_value(value *e) {
  if (e == NULL)
    return;
  switch (TYPEOF(e)) {
  case NUMBER: {
    float n = E_NUMBER(e);
    if (n - (float)(int)n == 0.0)
      printf("%d", (int)n);
    else
      printf("%f", n);
    break;
  }
  case SYMBOL:
    printf("%s", E_SYMBOL(e));
    break;
  case CELL:
    print_list(E_CELL(e));
    break;
  case LAMBDA:
    printf("LAMBDA ");
    print_list(E_LAMBDA(e)->args);
    break;
  case IFUNC:
    printf("IFUNC %p", E_IFUNC(e));
    break;
  case BOOLEAN:
    printf("%s", E_NUMBER(e) ? "#t" : "#f");
    break;
  case STRING:
    printf("%s", E_STRING(e));
    break;
  case CONTINUATION:
    printf("CONTINUATION");
    break;
  }
}
void print_list(cell *c) {
  printf("(");
  while (c != NULL) {
    print_value(c->car);
    if (TYPEOF(c->cdr) != CELL) {
      printf(" . ");
      print_value(c->cdr);
      break;
    }
    if (E_CELL(c->cdr) != NULL) {
      printf(" ");
    }
    c = E_CELL(c->cdr);
  }
  printf(")");
}
value *mk_number_value(float number) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = NUMBER;
  E_NUMBER(e) = number;
  return e;
}
value *mk_symbol_value(char *symbol) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = SYMBOL;
  char *s = xmalloc(strlen(symbol) + 1);
  strcpy(s, symbol);
  E_SYMBOL(e) = s;
  return e;
}
value *mk_empty_cell_value() {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = CELL;
  E_CELL(e) = NULL;
  return e;
}
value *mk_cell_value(value *car, value *cdr) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = CELL;
  E_CELL(e) = xmalloc(sizeof(cell));
  CAR(e) = car;
  CDR(e) = cdr;
  return e;
}
value *mk_lambda_value(cell *args, value *body, frame *env) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = LAMBDA;
  E_LAMBDA(e) = xmalloc(sizeof(lambda));
  E_LAMBDA(e)->args = args;
  E_LAMBDA(e)->body = body;
  E_LAMBDA(e)->env = env;
  return e;
}
value *mk_boolean_value(int b) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = BOOLEAN;
  E_BOOLEAN(e) = b;
  return e;
}
value *mk_ifunc_value(ifunc f) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = IFUNC;
  E_IFUNC(e) = f;
  return e;
}
value *mk_string_value(char *str) {
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = STRING;
  char *s = xmalloc(strlen(str) + 1);
  strcpy(s, str);
  E_STRING(e) = s;
  return e;
}
int cell_len(cell *c) {
  int len = 0;
  while (c != NULL) {
    len++;
    c = E_CELL(c->cdr);
  }
  return len;
}
int truish(value *e) {
  // truish: E → T
  // truish = λε . (ε = false → false, true)
  if (TYPEOF(e) == BOOLEAN) {
    return E_BOOLEAN(e);
  }
  return 1;
}
// Deep comparison of value* (numbers, symbols, lists, etc.)
int value_equal(value *a, value *b) {
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
      cell *ca = E_CELL(a), *cb = E_CELL(b);
      if (!ca && !cb) return 1;
      if (!ca || !cb) return 0;
      return value_equal(ca->car, cb->car) && value_equal(ca->cdr, cb->cdr);
    }
    default:
      return 0;
  }
}
