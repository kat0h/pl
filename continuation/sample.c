#include "continuation.h"
#include <stdio.h>

#define N_PRIME 1000 // 求める素数の個数
int number; // 素数の候補

int n_prime;
int prime[N_PRIME + 1];           // prime[n]はn番目の素数
continuation p_cont[N_PRIME + 1]; // 整除できるかを調べる継続
int candidate;                    // 素数の候補を示すフラグ

void is_prime(int n) {
  continuation temp;
  // 継続の取得
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
      is_prime(n_prime++); // 次の数を調べる
  } else {
    number += 1;
    candidate = 1;
    call_continuation(&p_cont[n_prime - 1], 1);
  }
  printf("......\n");
}
