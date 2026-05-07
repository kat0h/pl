#include <stdio.h>
#include "../parse.h"

int main() {
  value a = parse_program("(print (quote (1 2 3)))");
  print_value(a);
  puts("");
}
