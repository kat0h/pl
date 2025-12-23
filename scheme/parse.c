// parser
#include "main.h"
#include "parse.h"

char *input;

void skip_ws() {
  while (*input == ' ' || *input == '\n' || *input == '\t')
    input++;
}
int is_symbol_char() {
  return ('a' <= *input && *input <= 'z') || ('A' <= *input && *input <= 'Z') ||
         *input == '_' || *input == '!' || *input == '+' || *input == '-' ||
         *input == '=' || *input == '<' || *input == '>' || *input == '*' ||
         *input == '/' || ('0' <= *input && *input <= '9');
}
value *parse_hash_literal() {
  if (*input != '#')
    throw("parse error: not hash literal");
  input++;
  if (*input == 't') {
    input++;
    return mk_boolean_value(1);
  } else if (*input == 'f') {
    input++;
    return mk_boolean_value(0);
  } else {
    throw("parse error: unexpected token %c", *input);
  }
}
value *parse_list();
value *parse_value() {
#ifdef DEBUG
  printf("parse_value: %s\n", input);
#endif
  // number
  if ('0' <= *input && *input <= '9') {
    return mk_number_value(strtof(input, &input));
  // symbol
  } else if (is_symbol_char()) {
    char buf[SYMBOL_LEN_MAX];
    int i = 0;
    while (is_symbol_char()) {
      buf[i++] = *input++;
      if (i == SYMBOL_LEN_MAX - 1) {
        throw("symbol is too long");
      }
    }
    buf[i] = '\0';
    return mk_symbol_value(buf);
  } else if (*input == '`') {
    input++;
    // Parse quoted expression: '(...) => (quote ...)
    value *quoted = parse_value();
    value *quote_sym = mk_symbol_value("quote");
    value *list = mk_cell_value(quoted, mk_empty_cell_value());
    value *expr = mk_cell_value(quote_sym, list);
    return expr;
  } else if (*input == '"') {
    input++;
    char buf[SYMBOL_LEN_MAX];
    int i = 0;
    while (*input != '"') {
      buf[i++] = *input++;
      if (i == SYMBOL_LEN_MAX - 1) {
        throw("string is too long");
      }
    }
    input++;
    buf[i] = '\0';
    return mk_string_value(buf);
  } else if (*input == '#') {
    return parse_hash_literal();
  } else if (*input == '(') {
    input++;
    return parse_list();
  }
  throw("Unexpected valueession '%c' \"%s\"", *input, input);
}
value *parse_list() {
#ifdef DEBUG
  printf("parse_list: %s\n", input);
#endif
  skip_ws();
  if (*input == ')') {
    input++;
    return mk_empty_cell_value();
  }
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = CELL;
  E_CELL(e) = xmalloc(sizeof(cell));
  CAR(e) = parse_value();
  skip_ws();
  CDR(e) = parse_list();
  skip_ws();
  return e;
}
value *parse_paren() {
#ifdef DEBUG
  printf("parse_paren: %s\n", input);
#endif
  if (*input == '(') {
    input++;
    skip_ws();
    value *e = parse_list();
    skip_ws();
    return e;
  }
  throw("Unexpected token %c", *input);
}
value *parse_program_list() {
#ifdef DEBUG
  printf("parse_program_list: %s\n", input);
#endif
  if (*input == '\0')
    return mk_empty_cell_value();
  value *e = xmalloc(sizeof(value));
  TYPEOF(e) = CELL;
  E_CELL(e) = xmalloc(sizeof(cell));
  CAR(e) = parse_paren();
  skip_ws();
  CDR(e) = parse_program_list();
  skip_ws();
  return e;
}
value *parse_program(char *prg) {
#ifdef DEBUG
  printf("parse_program: %s\n", prg);
#endif
  input = prg;
  value *e = parse_program_list();
  if (*input != '\0') {
    throw("parser error input is not empty \"%s\"", input);
  }
  return e;
}

