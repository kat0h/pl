#include "main.h"
#include "repl.h"
#include "parse.h"
#include "value.h"

void repl() {
  puts("Interpriter");
  char buf[1024];
  frame *environ = mk_initial_env();
  printf("scm> ");
  while (fgets(buf, 1024, stdin) != NULL) {
    input = buf;
    if (*input == '\n') {
      printf("scm> ");
      continue;
    }
    value *program = parse_value();
    value *ret = eval_top(program, environ);
    printf("=> ");
    print_value(ret);
    puts("");
    printf("scm> ");
  }
}

