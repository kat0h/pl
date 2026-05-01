#ifndef VALUE_H
#define VALUE_H

// types
typedef struct Value value;
typedef struct Cell cell;
typedef struct Lambda lambda;
typedef struct Frame frame;
typedef struct KeyVal keyval;
typedef struct Continuation continuation;
typedef struct KV kv;
typedef value *(*ifunc)(value *, frame *);
struct Value {
  enum {
    NUMBER,
    SYMBOL,
    CELL,
    LAMBDA,
    IFUNC,
    BOOLEAN,
    STRING,
    CONTINUATION
  } type;
  union {
    float number;
    char *symbol;
    cell *cell;
    lambda *lmd;
    ifunc func;
    int boolean;
    char *string;
    continuation *cont;
  } body;
};
struct Cell {
  value *car;
  value *cdr;
};
int cell_len(cell *c);
struct Lambda {
  cell *args;
  value *body;
  frame *env;
};

void print_list(cell *c);
void print_value(value *e);
void print_list(cell *c);
value *mk_number_value(float number);
value *mk_symbol_value(char *symbol);
value *mk_empty_cell_value();
value *mk_cell_value(value *car, value *cdr);
value *mk_lambda_value(cell *args, value *body, frame *env);
value *mk_boolean_value(int b);
value *mk_ifunc_value(ifunc f);
value *mk_string_value(char *str);
int cell_len(cell *c);
int truish(value *e);
int value_equal(value *a, value *b);

#endif

