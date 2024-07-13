#include <alloca.h>
#include <stdio.h>
#include "continuation.h"
continuation cont;
int func2() {
  alloca(100);
  if (get_continuation(&cont) == 0) {
    puts("func2");
    return 0;
  } else {
    puts("func2 cont");
    return 1;
  }
}
int func() {
  alloca(100);
  return func2();
}
int main(int argc, char *argv[]) {
  INIT_CONTINUATION();
  puts("main");
  int a = 0;
  if (func() == 0) {
    puts("main_1");
    printf("a: %d\n", a);
    a++;
    call_continuation(&cont, 1);
  } else {
    puts("main_2");
    printf("a: %d\n", a);
  }
  free_continuation(&cont);
}
