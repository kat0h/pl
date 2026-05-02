#ifndef ENV_H
#define ENV_H
#include "value.h"

typedef struct KeyVal keyval;
typedef struct KV kv;
// environment
struct Frame {
  struct Frame *parent;
  kv *kv;
};
struct KV {
  char *key;
  value value;
  kv *next;
};

frame *make_frame(frame *parent);
void add_kv_to_frame(frame *env, char *symbol, value value);
kv *find_pair_in_current_frame(frame *env, char *symbol);
kv *find_pair_recursive(frame *env, char *symbol);
value define_to_env(frame *env, char *symbol, value value);
value set_to_env(frame *env, char *symbol, value value);
value lookup_frame(frame *env, char *symbol);
void print_frame(frame *env);

#endif
