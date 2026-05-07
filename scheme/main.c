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
    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
      perror("fopen");
      return 1;
    }
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(fsize + 1);
    if (buffer == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      fclose(fp);
      return 1;
    }
    fread(buffer, 1, fsize, fp);
    buffer[fsize] = '\0';
    fclose(fp);
    value program = parse_program(buffer);
    free(buffer);
    frame *environ = mk_initial_env();
    eval_list(program, environ, mk_empty_cell_value());
  }
}
#endif
