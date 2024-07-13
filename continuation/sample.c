#include <stdio.h>
#include "continuation.h"

#define N_PRIME 10  // 求める素数の個数
int number;         // 素数の候補

int n_prime;
int prime[N_PRIME+1];  // prime[n]はn番目の素数
continuation p_cont[N_PRIME+1]; // 整除できるかを調べる継続
int candidate; // 素数の候補を示すフラグ

int is_prime(int n) {
  continuation temp;
  // 継続の取得
  if (get_continuation(temp) == 0)
    p_cont[n] = temp;
  if (number % prime[n])
    call_continuation(p_cont[n-1], 1);

    
}

int main(int argc, char *argv[]) {
  INIT_CONTINUATION();
  
  free_continuation(cont);
}

