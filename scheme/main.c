#include <string.h>
#include <unistd.h>
#include <time.h>
#include "main.h"

#include "continuation.h"
#include "parse.h"
#include "repl.h"
#include "value.h"
#include "env.h"
#include "eval.h"

#ifndef TEST_BUILD
int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc < 2) {
    if (isatty(fileno(stdin)))
      repl();
    else
      throw("repl must be run in tty");
  } else {
    value *program = parse_program(argv[1]);
    frame *environ = mk_initial_env();
    eval_list(program, environ, mk_empty_cell_value());
  }
}
#endif
