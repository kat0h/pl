#ifndef VALUE_H
#define VALUE_H
typedef unsigned long value;

// based on https://github.com/tadd/schaf/blob/main/schaf.c
// VALUE
//  0b......000 Pointer

// flagsの下位6bitを型とする
#define TYPEMASK 0b111111
// Type
#define NUMBER       1
#define SYMBOL       2
#define CELL         3
#define LAMBDA       4
#define IFUNC        5
#define BOOLEAN      6
#define STRING       7
#define CONTINUATION 8

struct Header {
  unsigned long flags;
};
struct Number {
  unsigned long flags;
  float number;
};
struct Symbol {
  unsigned long flags;
  char *symbol;
};
struct Cell {
  unsigned long flags;
  value car;
  value cdr;
};
typedef struct Frame frame;
#include "env.h"
struct Lambda {
  unsigned long flags;
  struct Cell *args;
  value body;
  frame *env;
};
struct Boolean {
  unsigned long flags;
  int boolean;
};
typedef value (*ifunc)(value, frame *);
struct Ifunc {
  unsigned long flags;
  ifunc func;
};
struct String {
  unsigned long flags;
  char *string;
};
#include <setjmp.h>
struct Continuation {
  unsigned long flags;
  void *stack;
  unsigned long stacklen;
  void *rsp;
  jmp_buf cont_reg;
};

#define VALUEISAPOINTER(v) ((v & 0b111) == 0)
#define TYPEOF(v) (((struct Header*)v)->flags & TYPEMASK)

#define E_NUMBER(v)  (((struct Number*)v)->number)
#define E_SYMBOL(v)  (((struct Symbol*)v)->symbol)
#define E_CELL(v)    ((struct Cell*)v)
#define CELL_IS_EMPTY(v) \
  (v->car == (value)NULL && v->cdr == (value)NULL)
#define CAR(v)       (v->car)
#define CDR(v)       (v->cdr)
#define E_LAMBDA(v)  ((struct Lambda*)v)
#define E_IFUNC(v)   (((struct Ifunc*)v)->func)
#define E_BOOLEAN(v) (((struct Boolean*)v)->boolean)
#define E_STRING(v)  (((struct String*)v)->string)
#define E_CONTINUATION(v) (((struct Continuation*)v))

value mk_number_value(float number);
value mk_symbol_value(char *symbol);
value mk_cell_value(value car, value cdr);
value mk_empty_cell_value();
value mk_lambda_value(struct Cell *args, value body, frame *env);
value mk_ifunc_value(ifunc f);
value mk_boolean_value(int b);
value mk_string_value(char *str);
value mk_continuation_value();

void print_value(value v);
void print_list(struct Cell *c);
int cell_len(struct Cell *c);
int truish(value e);
int value_equal(value a, value b);

#endif

