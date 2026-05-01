#ifndef VALUE_H
#define VALUE_H

#define TYPEOF(x) (x->type)
#define E_NUMBER(x) (x->body.number)
#define E_SYMBOL(x) (x->body.symbol)
#define E_CELL(x) (x->body.cell)
#define E_LAMBDA(x) (x->body.lmd)
#define E_IFUNC(x) (x->body.func)
#define E_BOOLEAN(x) (x->body.boolean)
#define E_STRING(x) (x->body.string)
#define E_CONTINUATION(x) (x->body.cont)
#define CAR(x) (E_CELL(x)->car)
#define CDR(x) (E_CELL(x)->cdr)

// types
typedef struct Value value;
typedef struct Cell cell;
typedef struct Lambda lambda;
typedef struct Frame frame;
typedef struct Continuation continuation;
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

