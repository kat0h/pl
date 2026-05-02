#ifndef PARSE_H
#define PARSE_H
#include "value.h"

extern char *input;
value parse_program(char *prg);
value parse_value();

#endif
