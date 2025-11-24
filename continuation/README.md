# C言語で第一級継続を扱うライブラリ

## ファイルの説明

- test.sh
    - ライブラリの動作確認用
- prime.c
    - ライブラリを使って素数を計算
    - 多田先生のプログラムを流用
    - $CC prime.c continuation.c
- sample1.c
    - 継続の動作を分かりやすく確認
    - $CC sample1.c continuation.c

# 開発メモ

## コールスタックのアドレスを取得

```c
#define GETRSP(rsp) asm volatile("mov %%rsp, %0" : "=r"(rsp));
#define GETRBP(rbp) asm volatile("mov %%rbp, %0" : "=r"(rbp));
```

## alloca

alloca関数はcallerのスタックフレームからメモリを割り当てる関数。
よって、allocaを呼び出すとスタックのトップのアドレス(RSPレジスタの値)が増える。

```c
#include <alloca.h>
#include <stdio.h>
#define GETRSP(rsp) asm volatile("mov %%rsp, %0" : "=r"(rsp));

int main() {
  void *rsp;
  GETRSP(rsp);
  printf("%p\n", rsp);
  int *p = alloca(312);
  void *nrsp;
  GETRSP(nrsp);
  printf("%p\n", nrsp);

  printf("rsp-nrsp: %ld\n", (long)rsp - (long)nrsp);
  return 0;
}
```

## memmoveとmemcpy

memcpyはsrcとdstの区間に重複があるときの動作は未定義。memmoveは定義済み。
