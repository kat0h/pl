#include <stdio.h>
#include "../value.h"
#include "../env.h"
#include "../main.h"

int main() {
  // print_value
  // print_list etc...
  puts("expect: (1 . (2 . (3 . ())))");
  value v = mk_cell_value(
      mk_number_value(1.0),
      mk_cell_value(
        mk_number_value(2.0),
        mk_cell_value(
          mk_number_value(3.0),
          mk_empty_cell_value())));
  print_value(v);puts("");
  // cell_len
  puts("expect: 3");
  printf("%d\n", cell_len(E_CELL(v)));
  // truish
  puts("expect: #f");
  value i = mk_boolean_value(0);
  print_value(i);puts("");
  // deep compare
  value w = mk_cell_value(
      mk_number_value(1.0),
      mk_cell_value(
        mk_number_value(2.0),
        mk_cell_value(
          mk_number_value(3.0),
          mk_empty_cell_value())));
  puts("expect: 1");
  printf("%d\n", value_equal(v, w));
}
