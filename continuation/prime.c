#include "continuation.h"
#include <stdio.h>

// 移植性のあるCの継続ライブラリ 多田好克 より
// https://ipsj.ixsq.nii.ac.jp/records/30459
// 一部改変

#define N_PRIME 10000
int number;

int n_prime;
int prime[N_PRIME + 1];
continuation p_cont[N_PRIME + 1];
int candidate;

void is_prime(int n) {
  continuation temp;
  if (get_continuation(&temp) == 0)
    p_cont[n] = temp;
  if (number % prime[n]) {
    call_continuation(&p_cont[n - 1], 1);
  } else {
    candidate = 0;
    call_continuation(&p_cont[0], 1);
  }
}

int main(int argc, char *argv[]) {
  INIT_CONTINUATION();

  n_prime = 1;
  number = 2;
  candidate = 1;
  get_continuation(&p_cont[0]);
  if (candidate) {
    prime[n_prime] = number++;
    printf("%d, ", prime[n_prime]);
    fflush(stdout);
    if (n_prime < N_PRIME)
      is_prime(n_prime++);
  } else {
    number += 1;
    candidate = 1;
    call_continuation(&p_cont[n_prime - 1], 1);
  }
  printf("......\n");
}
