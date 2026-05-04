#include <stdio.h>
#include "value.h"
#include "env.h"

int main() {
  // print_value
  // print_list etc...
  value v = mk_cell_value(
      mk_number_value(1.0),
      mk_cell_value(
        mk_number_value(2.0),
        mk_cell_value(
          mk_number_value(3.0),
          mk_empty_cell_value())));
  print_value(v);puts("");
}
