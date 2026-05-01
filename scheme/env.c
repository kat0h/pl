#include <stdio.h>
#include <string.h>
#include "env.h"

#include "main.h"
#include "value.h"
#include "memory.h"

// env
frame *make_frame(frame *parent) {
  frame *f = xmalloc(sizeof(frame));
  f->parent = parent;
  f->kv = NULL;
  return f;
}
void add_kv_to_frame(frame *env, char *symbol, value *value) {
  kv *i = xmalloc(sizeof(kv));
  i->key = symbol;
  i->value = value;
  i->next = env->kv;
  env->kv = i;
}
kv *find_pair_in_current_frame(frame *env, char *symbol) {
  kv *i = env->kv;
  while (i != NULL) {
    if (strcmp(i->key, symbol) == 0)
      return i;
    i = i->next;
  }
  return NULL;
}
kv *find_pair_recursive(frame *env, char *symbol) {
  if (env == NULL) {
    throw("symbol %s not found", symbol);
  }
  kv *i = find_pair_in_current_frame(env, symbol);
  if (i != NULL)
    return i;
  return find_pair_recursive(env->parent, symbol);
}
value *define_to_env(frame *env, char *symbol, value *value) {
  add_kv_to_frame(env, symbol, value);
  return mk_symbol_value(symbol);
}
value *set_to_env(frame *env, char *symbol, value *value) {
  kv *i = find_pair_recursive(env, symbol);
  if (i == NULL)
    throw("symbol %s not found", symbol);
  i->value = value;
  return mk_symbol_value(symbol);
}
value *lookup_frame(frame *env, char *symbol) {
  kv *v = find_pair_recursive(env, symbol);
  return v->value;
}
void print_frame(frame *env) {
  while (env != NULL) {
    puts("....Frame...........................................");
    kv *i = env->kv;
    while (i != NULL) {
      printf("  %s: ", i->key);
      print_value(i->value);
      puts("");
      i = i->next;
    }
    env = env->parent;
  }
  puts("....................................................\n");
}

