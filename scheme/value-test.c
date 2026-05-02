#include <stdio.h>
#include "value.h"
#include "env.h"

int main() {
  value a = mk_number_value(1.234);
  print_value(a);
  puts("");
  value b = mk_symbol_value("hello");
  print_value(b);
  puts("");
  value c = mk_cell_value(a, mk_cell_value(b, mk_empty_cell_value()));
  print_value(c);
  puts("");
  printf("cell len %d \n", cell_len(E_CELL(c)));
  value l = mk_lambda_value(E_CELL(c), b, make_frame(NULL));
  print_value(l);
  puts("");
  value bo = mk_boolean_value(1);
  print_value(bo);
  puts("");
  printf("truish %d \n", truish(bo));
  value f = mk_ifunc_value(NULL);
  print_value(f);
  puts("");
  value str = mk_string_value("hello string");
  print_value(str);
  puts("");
  value cont = mk_continuation_value();
  print_value(cont);
  puts("");
}
